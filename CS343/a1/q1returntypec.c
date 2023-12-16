#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef enum Type {Ok, Ex1, Ex2, Ex3} Type;

typedef union Value {
  double value;
  short int ex1_code;
  int ex2_code;
  long int ex3_code;
} Value;

typedef struct Result {
  Type type;
  Value value;
} Result;


intmax_t eperiod = 10000;								// exception period
int randcnt = 0;
int Rand() { randcnt += 1; return rand(); }

Result rtn1( double i ) {
  Result result;
  if ( Rand() % eperiod == 0 ) {
    result.type = Ex1;
    result.value.ex1_code = (short int)Rand();
    return result;
  }

  result.type = Ok;
  result.value.value = i + Rand();
  return result;
}
Result rtn2( double i ) {
  Result result;

  if ( Rand() % eperiod == 0 ) {
    result.type = Ex2;
    result.value.ex2_code = (int)Rand();
    return result;
  }

  Result rtn1_result = rtn1(i);

  if (rtn1_result.type == Ex1) {
    return rtn1_result;
  }

  result.type = Ok;
  result.value.value = rtn1_result.value.value + Rand();
	return result;
}
Result rtn3( double i ) {
  Result result;
  if ( Rand() % eperiod == 0 ) {
    result.type = Ex3;
    result.value.ex3_code = (long int)Rand();
    return result;
  }

  Result rtn2_result = rtn2(i);

  if (rtn2_result.type == Ex1 || rtn2_result.type == Ex2) {
    return rtn2_result;
  }

  result.type = Ok;
  result.value.value = rtn2_result.value.value + Rand();
	return result;
}

void* error_label;

intmax_t convert( const char * str ) {			// convert C string to integer
	char * endptr;
	errno = 0;											// reset
	intmax_t val = strtoll( str, &endptr, 10 );			// attempt conversion
	if ( errno == ERANGE ) goto *error_label;
	if ( endptr == str ||								// conversion failed, no characters generated
		 *endptr != '\0' ) goto *error_label; // not at end of str ?
	return val;
} // convert

int main( int argc, char * argv[] ) {
	intmax_t times = 100000000, seed = getpid();		// default values

  error_label = &&L1;

  switch ( argc ) {
    case 4: if ( strcmp( argv[3], "d" ) != 0 ) {	// default ?
    seed = convert( argv[3] ); if ( seed <= 0 ) goto L1; }
    case 3: if ( strcmp( argv[2], "d" ) != 0 ) {	// default ?
    eperiod = convert( argv[2] ); if ( eperiod <= 0 ) goto L1; }
    case 2: if ( strcmp( argv[1], "d" ) != 0 ) {	// default ?
    times = convert( argv[1] ); if ( times <= 0 ) goto L1; }
    case 1: break;								// use all defaults
    default: goto L1;
  } // switch
  goto S1;

L1:
  printf("Usage %s [ times > 0 | d [ eperiod > 0 | d [ seed > 0 | d ] ] ]\n", argv[0]);
  exit( EXIT_FAILURE );
S1:
	srand( seed );

	double rv = 0.0;
	int ev1 = 0, ev2 = 0, ev3 = 0;
	int rc = 0, ec1 = 0, ec2 = 0, ec3 = 0;

	for ( int i = 0; i < times; i += 1 ) {
    Result result = rtn3(i);

    if (result.type == Ex1) {
      ev1 += result.value.ex1_code;
      ec1 += 1;
    } else if (result.type == Ex2) {
      ev2 += result.value.ex2_code;
      ec2 += 1;
    } else if (result.type == Ex3) {
      ev3 += result.value.ex3_code;
      ec3 += 1;
    } else {
      rv += result.value.value;
      rc += 1;
    }
	} // for

  printf("randcnt %d\n", randcnt);
  printf("normal result %g exception results %d %d %d\n", rv, ev1, ev2, ev3);
  printf("calls %d exceptions %d %d %d\n", rc, ec1, ec2, ec3);
}