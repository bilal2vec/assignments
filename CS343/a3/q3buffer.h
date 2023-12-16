#include <iostream>

using namespace std;

#ifdef BUSY							// busy waiting implementation
template<typename T> class BoundedBuffer {
	// We use these variables to create a circular buffer
  T *buffer;
  const unsigned int size;
  unsigned int write_idx;
  unsigned int read_idx;
	unsigned long int n_blocks;
	bool poisoned;

	// OwnerLock mutex to protect the buffer's critical sections
	uOwnerLock buffer_lock;
	// CondLocks to keep track of producers and consumers.
	uCondLock prod_lock;
	uCondLock cons_lock;

  public:
	_Event Poison {};
	BoundedBuffer( const unsigned int size = 10) : size(size) { 
		// Circular buffers with size + 1 elements can store a maximum of size elements
    buffer = new T[size + 1];
    write_idx = 0;
    read_idx = 0;
		n_blocks = 0;
		poisoned = false;
  }

	// Just return the number of n_blocks
	unsigned long int blocks() { return n_blocks; }

	// Aquire the buffer lock, set the poison member variable and release it
	void poison() {
		buffer_lock.acquire();
		poisoned = true;
		buffer_lock.release();
	}

	void insert( T elem ) {
		// Acquire the buffer lock
		buffer_lock.acquire();

		// Busy wait until there is space in the buffer
		// Increment n_blocks to keep track of the number of waits
		while ((write_idx + 1) % size == read_idx) {
			n_blocks++;
			prod_lock.wait(buffer_lock);
		}

		// Assert the buffer is not full
		unsigned int current_size = (write_idx - read_idx + size) % size;
		assert(current_size < size);

		// Actualy insert the element
		buffer[write_idx] = elem;
		write_idx = (write_idx + 1) % size;

		// Signal to the consumers that there is a new element ready to consume
		cons_lock.signal();

		// Release the buffer lock
		buffer_lock.release();
	}

	T remove() __attribute__(( warn_unused_result )) {
		// Acquire the buffer lock
		buffer_lock.acquire();

		// Busy wait until there is an element to consume
		// Note that this checks for the size of the buffer being zero
		while (read_idx == write_idx) {
			// Edge case that if the buffer is poisoned and is empty, then release all locks and throw a poison exception
			if (poisoned) {
				cons_lock.broadcast();
				buffer_lock.release();
				_Throw Poison();
			} else {
				n_blocks++;
				cons_lock.wait(buffer_lock);
			}
		}

		// Assert the buffer is not empty
		unsigned int current_size = (write_idx - read_idx + size) % size;
		assert(current_size > 0);

		// Actualy remove the element
		T elem = buffer[read_idx];
		read_idx = (read_idx + 1) % size;

		// Signal to producer
		prod_lock.signal();

		// Release the buffer lock
		buffer_lock.release();

		// Return the element
		return elem;
	}

	// Delete heap-allocated array
	~BoundedBuffer() { delete[] buffer; }
};
#endif // BUSY

#ifdef NOBUSY							// no busy waiting implementation
#include "BargingCheck.h"
template<typename T> class BoundedBuffer {
	BCHECK_DECL;

  T *buffer;
  const unsigned int size;
  unsigned int write_idx;
  unsigned int read_idx;
	unsigned long int n_blocks;
	bool poisoned;

	uOwnerLock buffer_lock;
	uCondLock prod_lock;
	uCondLock cons_lock;
 
	// Flag to keep track of whether a thread is signalling
	bool is_signalling;
	// CondLock to prevent new threads from barging in while an existing thread is signalling
	uCondLock barging_lock;

  public:
  _Event Poison {};
	
	BoundedBuffer( const unsigned int size = 10) : size(size) { 
    buffer = new T[size + 1];
    write_idx = 0;
    read_idx = 0;
		n_blocks = 0;
		poisoned = false;

		// Initially no thread is signalling
		is_signalling = false;
  }

  unsigned long int blocks() {
		return n_blocks;
	}

	void poison() {
		buffer_lock.acquire();
		poisoned = true;
		buffer_lock.release();
	}

	void insert( T elem ) {
		buffer_lock.acquire();
		PROD_ENTER;

		try {
			// test the signalling flag to know if this thread would be barging in, and if so, wait on the barging lock until we are told that all other tasks have already been unblocked.
			if (is_signalling) {
				barging_lock.wait(buffer_lock);
			}

			if ((write_idx + 1) % size == read_idx) {

				// If the buffer is full, then we can unblock a new thread (if it exists), and update the signalling flag appropriately
				if (!barging_lock.signal()) {
					is_signalling = false;
				} else {
					is_signalling = true;
				}

				n_blocks++;
				prod_lock.wait(buffer_lock);
			}

			unsigned int current_size = (write_idx - read_idx + size) % size;
			assert(current_size < size);

			buffer[write_idx] = elem;
			INSERT_DONE;
			write_idx = (write_idx + 1) % size;

			// Set the signalling flag to true in anticipation of signalling a thread
			// Either we will signal an existing consumer thread or a new producer/consumer thread
			is_signalling = true;

			// Signal an existing consumer thread if one exists
			CONS_SIGNAL(cons_lock);
			if (!cons_lock.signal()) {
				// If not, then signal a new producer/consumer thread if one exists
				if (!barging_lock.signal()) {
					// If not, then set the signalling flag to false
					is_signalling = false;
				}
			}

		} _Finally {
			buffer_lock.release();
		}
	}

	T remove() __attribute__(( warn_unused_result )) {
		buffer_lock.acquire();
		CONS_ENTER;

		T elem;

		try {
			// test the signalling flag to know if this thread would be barging in, and if so, wait on the barging lock until we are told that all other tasks have already been unblocked.
			if (is_signalling) {
				barging_lock.wait(buffer_lock);

				if (poisoned) {
					// Unblock all potentially barging threads, with the signalling flag set to false since they will all just throw a poison exception
					is_signalling = false;
					barging_lock.broadcast();

					cons_lock.broadcast();
					_Throw Poison();
				} 

			}

			if (read_idx == write_idx) {
				// We have to do two checks for a poisoned buffer when the buffer is empty, one before and one after the wait to handle the cases where we may or may not have consumer(s) waiting on the buffer.
				if (poisoned) {
					// Unblock all potentially barging threads, with the signalling flag set to false since they will all just throw a poison exception
					is_signalling = false;
					barging_lock.broadcast();

					cons_lock.broadcast();
					_Throw Poison();
				} 

				// If the buffer is full, then we can unblock a new thread (if it exists), and update the signalling flag appropriately
				if (!barging_lock.signal()) {
					is_signalling = false;
				} else {
					is_signalling = true;
				}

				n_blocks++;
				cons_lock.wait(buffer_lock);

				// Check for a poisoned buffer again after the wait, to handle the case where we have consumer(s) waiting on the buffer.
				if (poisoned) {
					is_signalling = false;
					barging_lock.broadcast();

					cons_lock.broadcast();
					_Throw Poison();
				} 

			}

			int current_size = (write_idx - read_idx + size) % size;
			assert(current_size >= 0);

			elem = buffer[read_idx];
			REMOVE_DONE;
			read_idx = (read_idx + 1) % size;

			// Set the signalling flag to true in anticipation of signalling a thread
			// Either we will signal an existing consumer thread or a new producer/consumer thread
			is_signalling = true;
			
			// Signal an existing consumer thread if one exists
			PROD_SIGNAL(prod_lock);
			if (!prod_lock.signal()) {
				// If not, then signal a new producer/consumer thread if one exists
				if (!barging_lock.signal()) {
					// If not, then set the signalling flag to false
					is_signalling = false;
				}
			}

		} _Finally {
			buffer_lock.release();
		}

		return elem;
	}

	~BoundedBuffer() { delete[] buffer; }
};
#endif // NOBUSY

