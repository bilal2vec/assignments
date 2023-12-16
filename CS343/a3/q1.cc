#include <iostream>
#include <vector>
#include <memory>										// unique_ptr
using namespace std;

intmax_t tasks = 1, times = 200'000'000, asize = 10;	// default values

_Task Worker {
	void main() {
		for ( int t = 0; t < times; t += 1 ) {
		#if defined( STACK )
			volatile int arr[asize] __attribute__ (( unused )); // prevent unused warning
			for ( int i = 0; i < asize; i += 1 ) arr[i] = i;
		#elif defined( DARRAY )
			unique_ptr<volatile int []> arr( new volatile int[asize] );
			for ( int i = 0; i < asize; i += 1 ) arr[i] = i;
		#elif defined( VECTOR1 )
			vector<int> arr( asize );
			for ( int i = 0; i < asize; i += 1 ) arr.at(i) = i;
			asm volatile( "" :: "r"(arr.data()):"memory" ); // prevent eliding code
		#elif defined( VECTOR2 )
			vector<int> arr;
			for ( int i = 0; i < asize; i += 1 ) arr.push_back(i);
			asm volatile( "" :: "r"(arr.data()):"memory" ); // prevent eliding code
		#else
			#error unknown data structure
		#endif
		} // for
	} // Worker::main
}; // Worker

int main( int argc, char * argv[] ) {
	bool nosummary = getenv( "NOSUMMARY" );				// print heap statistics ?

	try {												// process command-line arguments
		switch ( argc ) {
		  case 4:
			asize = convert( argv[3] ); if ( asize <= 0 ) throw 1;
		  case 3:
			times = convert( argv[2] ); if ( times <= 0 ) throw 1;
		  case 2:
			tasks = convert( argv[1] ); if ( tasks <= 0 ) throw 1;
		} // switch
	} catch( ... ) {
		cout << "Usage: " << argv[0] << " [ tasks (> 0) [ times (> 0) [ array size (> 0) ] ] ]" << endl;
		exit( 1 );
	} // try
	uProcessor p[tasks - 1];							// add CPUs (start with one)
	{
		Worker workers[tasks];							// add threads
	}
	if ( ! nosummary ) { malloc_stats(); }
} // main