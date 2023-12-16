#include <iostream>
#include <cstdlib>										// access: rand, srand
#include <cstring>										// access: strcmp
#include <variant>
using namespace std;
#include <unistd.h>										// access: getpid

struct Ex1 { short int code; };
struct Ex2 { int code; };
struct Ex3 { long int code; };

intmax_t eperiod = 10000;								// exception period
int randcnt = 0;
int Rand() { randcnt += 1; return rand(); }

variant<double, Ex1> rtn1( double i ) {
  if ( Rand() % eperiod == 0 ) { return Ex1{ (short int)Rand() }; }
	return i + Rand();
}
variant<double, Ex1, Ex2> rtn2( double i ) {
  if ( Rand() % eperiod == 0 ) { return Ex2{ Rand() }; }

  variant<double, Ex1> rtn1_result = rtn1(i);

  if (holds_alternative<Ex1>(rtn1_result)) {
    return get<Ex1>(rtn1_result);
  }

	return get<double>(rtn1_result) + Rand();
}
variant<double, Ex1, Ex2, Ex3> rtn3( double i ) {
  if ( Rand() % eperiod == 0 ) { return Ex3{ Rand() }; }

  variant<double, Ex1, Ex2> rtn2_result = rtn2(i);

  if (holds_alternative<Ex1>(rtn2_result)) {
    return get<Ex1>(rtn2_result);
  } else if (holds_alternative<Ex2>(rtn2_result)) {
    return get<Ex2>(rtn2_result);
  }

	return get<double>(rtn2_result) + Rand();
}

static intmax_t convert( const char * str ) {			// convert C string to integer
	char * endptr;
	errno = 0;											// reset
	intmax_t val = strtoll( str, &endptr, 10 );			// attempt conversion
	if ( errno == ERANGE ) throw std::out_of_range("");
	if ( endptr == str ||								// conversion failed, no characters generated
		 *endptr != '\0' ) throw std::invalid_argument(""); // not at end of str ?
	return val;
} // convert

int main( int argc, char * argv[] ) {
	intmax_t times = 100000000, seed = getpid();		// default values
	struct cmd_error {};
	try {
		switch ( argc ) {
		  case 4: if ( strcmp( argv[3], "d" ) != 0 ) {	// default ?
			seed = convert( argv[3] ); if ( seed <= 0 ) throw cmd_error(); }
		  case 3: if ( strcmp( argv[2], "d" ) != 0 ) {	// default ?
			eperiod = convert( argv[2] ); if ( eperiod <= 0 ) throw cmd_error(); }
		  case 2: if ( strcmp( argv[1], "d" ) != 0 ) {	// default ?
			times = convert( argv[1] ); if ( times <= 0 ) throw cmd_error(); }
		  case 1: break;								// use all defaults
		  default: throw cmd_error();
		} // switch
	} catch( ... ) {
		cerr << "Usage: " << argv[0] << " [ times > 0 | d [ eperiod > 0 | d [ seed > 0 | d ] ] ]" << endl;
		exit( EXIT_FAILURE );
	} // try
	srand( seed );

	double rv = 0.0;
	int ev1 = 0, ev2 = 0, ev3 = 0;
	int rc = 0, ec1 = 0, ec2 = 0, ec3 = 0;

	for ( int i = 0; i < times; i += 1 ) {

    variant<double, Ex1, Ex2, Ex3> rtn3_result = rtn3(i);

    if (holds_alternative<Ex1>(rtn3_result)) {
      ev1 += get<Ex1>(rtn3_result).code;
      ec1 += 1;
    } else if (holds_alternative<Ex2>(rtn3_result)) {
      ev2 += get<Ex2>(rtn3_result).code;
      ec2 += 1;
    } else if (holds_alternative<Ex3>(rtn3_result)) {
      ev3 += get<Ex3>(rtn3_result).code;
      ec3 += 1;
    } else {
       rv += get<double>(rtn3_result);
       rc += 1;
    }
	} // for
	cout << "randcnt " << randcnt << endl;
	cout << "normal result " << rv << " exception results " << ev1 << ' ' << ev2 << ' ' << ev3 << endl;
	cout << "calls "  << rc << " exceptions " << ec1 << ' ' << ec2 << ' ' << ec3 << endl;
}