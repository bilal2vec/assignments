#include <iostream>
#include <string>
#include <sstream>
using namespace std;
#include <malloc.h>										// malloc_stats

int main( int argc, char * argv[] ) {
	intmax_t times = 1'000'000, size = 20;				// defaults
	bool nosummary = getenv( "NOSUMMARY" );				// print summary ?
	struct cmd_error {};								// command-line errors

	try {
		switch ( argc ) {
		  case 3: size = convert( argv[2] ); if ( size <= 0 ) { throw cmd_error(); }
		  case 2: times = convert( argv[1] ); if ( times <= 0 ) { throw cmd_error(); }
		  case 1: break;								// use defaults
		  default: throw cmd_error();
		} // switch
	} catch( ... ) {
		cerr << "Usage: " << argv[0] << " [ times (> 0) [ size (> 0) ] ]" << endl;
		exit( EXIT_FAILURE );
	} // try

	enum { C = 1'234'567'890 };							// print multiple characters

	#if defined( ARRAY )
	struct S { long int i, j, k, l; };
	S buf[size];										// internal-data buffer
	#elif defined( STRING )
	string strbuf;										// external-data buffer
	#elif defined( STRSTREAM )
	stringstream ssbuf;									// external-data buffer
	#else
		#error unknown buffering style
	#endif

	for ( int i = 0; i < times; i += 1 ) {
		#if defined( ARRAY )
		for ( volatile int i = 0; i < size; i += 1 ) buf[i] = (S){ C - i, C + i, C | i, C ^ i };
		#elif defined( STRING )
		for ( volatile int i = 0; i < size; i += 1 ) strbuf += to_string(C - i) + '\t' + to_string(C + i) + '\t'
												   + to_string(C | i) + '\t' + to_string(C ^ i) + '\t';
		strbuf.clear();									// reset string
		#elif defined( STRSTREAM )
		for ( volatile int i = 0; i < size; i += 1 ) ssbuf << (C - i) << '\t' << (C + i) << '\t'
												  << (C | i) << '\t' << (C ^ i) << '\t';
		ssbuf.seekp( 0 );								// reset stream
		#else
			#error unknown buffering style
		#endif
	} // for

	if ( ! nosummary ) { malloc_stats(); }				// print heap statistics
}