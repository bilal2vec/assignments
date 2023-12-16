#include <iostream>

using namespace std;

template<typename T> _Coroutine Binsertsort {
	T value;					// communication: value passed down/up tree
	void main() {
        try {
            /* This enable/disable pair is set up to catch a sentinel thrown by
            its starter when the coroutine is first resumed. This only happens
            in the special case where the list has zero elements, main will
            throw a sentinel before calling .sort() on the first element. No
            Bininsertsort coroutine will throw a sentinel to a child coroutine
            before calling .sort() on it (i.e. leaf nodes will always have a
            value) */
            _Enable;
            _Disable;

            T pivot;

            try {
                _Enable {
                    // Get pivot value and suspend
                    pivot = value;
                    suspend();
                }
            /* This catch block handles the case where a Bininsertsort
            coroutine / node in the BST is a leaf node. It immediately
            throws a sentinel to its parent coroutine / node to indicate
            that it is done sorting its values. */
            } catch(Sentinel &e) {
                suspend();
                _Throw Sentinel();
            }

            /* Create left and right branches on stack only if this node is not
            a leaf node */
            Binsertsort<T> left;
            Binsertsort<T> right;

            // Add values to left and right branches
            try {
                _Enable {
                    while (1) {
                        if (value < pivot) {
                            left.sort(value);
                        } else {
                            right.sort(value);
                        }
                        suspend();
                    }
                }
            /* This catch block handles the general case where a Bininsertsort
            coroutine / node in the BST is not a leaf node. It throws a
            sentinel to its left coroutine / node and calls .retrieve on its
            left coroutine / node to repeatedly retrieve values from it in
            sorted order until its left coroutine / node throws a sentinel back
            to it indicating that it is done sorting its values. It then passes
            up its pivot value before repeating the same procedure to pass up
            its right coroutine / node's sorted values. */
            } catch(Sentinel &e) {
                try {
                    _Enable {
                        _Resume Sentinel() _At left;

                        while (1) {
                            value = left.retrieve();
                            suspend();
                        }
                    }
                } catch (Sentinel &e) {
                    // catch statements required to break out of while loop 
                }

                // Pass up pivot value
                value = pivot;
                suspend();

                try {
                    _Enable {
                        _Resume Sentinel() _At right;

                        while (1) {
                            value = right.retrieve();
                            suspend();
                        }
                    }
                } catch (Sentinel &e) {
                    // catch statements required to break out of while loop 
                }
            }
        } catch(Sentinel &e) {
            // catch statement to automatically terminate coroutine
        }

        // A final sentinel is raised at the end of the coroutine to terminate.
        _Resume Sentinel() _At resumer();
	}
  public:
	_Event Sentinel {};
	void sort( T value ) {		// value to be sorted
		Binsertsort<T>::value = value;
		resume();
	}
	T retrieve() {				// retrieve sorted value
		resume();
		return value;
	}
};