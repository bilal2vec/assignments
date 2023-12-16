#ifndef _q3floatconstant_h
#define _q3floatconstant_h

_Coroutine FloatConstant {
	char ch;											// character passed by cocaller
	
	int get_sign();
	void skip_char(char c);
	void throw_error();
	void throw_match(double value);

	void main();										// coroutine main
  public:
	enum { EOT = '\003' };								// end of text
	_Event Match {										// last character match
	  public:
		double value;									// floating-point value
		Match( double value ) : value( value ) {}
	};
	_Event Error {};									// last character invalid

	void next( char c ) {
		ch = c;											// communication input
		resume();										// activate
	}
};

// Test data

// +12.E-2
// -12.5
// 12.
// -.5
// .1e+123
// -12.5F
// 002.l
// +01234567890123456.

// a
// +.
//  12.0
// 12.0   
// 1.2.0a
// - 12.5F
// 123.ff
// 0123456789.01234567E-0124

#endif
/* q3floatconstant.h */