#include <iostream>

#include "q3buffer.h"

#include <uPRNG.h>
#include <uActor.h>

using namespace std;

_Task Producer {
	BoundedBuffer<int> &buffer;
	const int Produce;
	const int Delay;

	// Produce items, delay for a random time interval [0,Delay) after each, and insert them into the buffer 
	void main() {
		for (int i = 0; i < Produce; i++) {
			int n_delays = prng(Delay);
			yield(n_delays);

			buffer.insert(i + 1);
		}
	}
public:
	Producer( BoundedBuffer<int> & buffer, const int Produce, const int Delay) : buffer(buffer), Produce(Produce), Delay(Delay) {}
};

_Task Consumer {
	BoundedBuffer<int> &buffer;
	const int Delay;
	int &sum;


	// Remove items from the buffer and add them to sum, delay for a random time interval [0,Delay) after each
	void main() {
		try {
			_Enable {
				while (true) {
					int n_delays = prng(Delay);
					yield(n_delays);

					sum += buffer.remove();
				}
			}
		// If the buffer is poisoned, stop consuming
		} catch (BoundedBuffer<int>::Poison& e) {
			return;
		}
	}
public:
	Consumer( BoundedBuffer<int> & buffer, const int Delay, int &sum) : buffer(buffer), Delay(Delay), sum(sum) {}
};

int main(int argc, char* argv[]) {
	struct cmd_error {};

	// Default values
	int cons = 5;
	int prods = 3;
	int produce = 10;
	int buffersize = 10;
	int delays = 0;
	int processors = 1;

	// Parse arguments
	try {
		switch (argc) {
			case 7:
				if (strcmp(argv[6], "d") != 0) {
					processors = convert(argv[6]);
					if (processors <= 0) throw cmd_error();
				}
			case 6:
				if (strcmp(argv[5], "d") != 0) {
					delays = convert(argv[5]);
					if (delays <= 0) throw cmd_error();
				}
			case 5:
				if (strcmp(argv[4], "d") != 0) {
					buffersize = convert(argv[4]);
					if (buffersize <= 0) throw cmd_error();
				}
			case 4:
				if (strcmp(argv[3], "d") != 0) {
					produce = convert(argv[3]);
					if (produce <= 0) throw cmd_error();
				}
			case 3:
				if (strcmp(argv[2], "d") != 0) {
					prods = convert(argv[2]);
					if (prods <= 0) throw cmd_error();
				}
			case 2:
				if (strcmp(argv[1], "d") != 0) {
					cons = convert(argv[1]);
					if (cons <= 0) throw cmd_error();
				}
			case 1:
				break;
			default:
				throw cmd_error();
		}
	} catch (...) {
		cout << "Usage: " << argv[0] << " [ Cons (> 0) | 'd' (default 5) [ Prods (> 0) | 'd' (default 3) [ Produce (> 0) | 'd' (default 10) [ BufferSize (> 0) | 'd' (default 10) [ Delay (> 0) | 'd' (default 8) [ Processors (> 0) | 'd' (default 1) ] ] ] ] ] ]" << endl;
		exit(EXIT_FAILURE);
	}

	// If delays is 0 (sentinel default value), set it to the sum of producers and consumers
	if (delays == 0) {
		delays = cons + prods;
	}

	// Set the number of processors
	uProcessor p[processors - 1] __attribute__(( unused ));

	// Initialize the buffer
	BoundedBuffer<int> buffer(buffersize);

	// Initialize the subtotal counters
	int subtotal_counters[cons];

	for (int i = 0; i < cons; i++) {
		subtotal_counters[i] = 0;
	}

	// Create the producers and consumers
	Producer* producers[prods];
	Consumer* consumers[cons];

	for (int i = 0; i < prods; i++) {
		producers[i] = new Producer(buffer, produce, delays);
	}

	for (int i = 0; i < cons; i++) {
		consumers[i] = new Consumer(buffer, delays, subtotal_counters[i]);
	}

	// Wait for the producers to finish
	for (int i = 0; i < prods; i++) {
		delete producers[i];
	}

	// Poison the buffer
	buffer.poison();

	// Wait for the consumers to finish
	for (int i = 0; i < cons; i++) {
		delete consumers[i];
	}

	// Sum up the subtotals and print
	int total_sum = 0;
	for (int i = 0; i < cons; i++) {
		total_sum += subtotal_counters[i];
	}

	cout << "total: " << total_sum << endl;


	// cout << "blocks: " << buffer.blocks() << endl;

	return 0;
}