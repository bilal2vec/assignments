#ifdef __cforall
#include <fstream.hfa>
#else
#include <iostream>
#endif // cforall

// Preprocessor variables declared in a program can have no value (empty), but variables declared with compiler flag -D
// have a value of one, unless explicitly set. So the following trick concatenates a 0 at the end of the variable, so if
// the variable has no value it becomes 0. Otherwise, the value is increased by 10. For example, if the variable is 1,
// it becomes 10 => do the abort; any other value means do not do the abort.

#define DO_EXPAND( VAL ) VAL ## 0
#define EXPAND( VAL ) DO_EXPAND( VAL )

#ifdef BARGINGCHECK
	#define BCHECK( stmt ) stmt
	#if EXPAND(BARGINGCHECK) != 10
		#define __BCHECK_PRT__( msg ) abort( msg )
	#else
#ifdef __cforall
		#define __BCHECK_PRT__( msg ) mutex( serr ) serr | msg;
#else
		#define __BCHECK_PRT__( msg ) std::osacquire( std::cerr ) << msg << std::endl
#endif // cforall
	#endif
#else
	#define BCHECK( stmt )
#endif // BARGINGCHECK

#define __BCHECK_DECL__ \
unsigned int __consEnterC__ = 0, __prodEnterC__ = 0, __removedoneC__ = 0, __insertdoneC__ = 0; \
unsigned int __next__ = 0; \
\
unsigned int __prodEnter__() { \
	__prodEnterC__ += 1; \
	return __prodEnterC__ - 1; \
} \
void __insertdone__( unsigned int ticket ) { \
	if ( ticket != __insertdoneC__ ) { __BCHECK_PRT__( "**** BARGING ERROR **** producer over producer" ); } \
	if ( __next__ == 2 ) { __BCHECK_PRT__( "**** BARGING ERROR **** producer over consumer" ); } \
	__next__ = 0; \
	__insertdoneC__ += 1; \
} \
void __prodSignal__( uCondLock & p ) { \
	if ( ! p.empty() ) { __next__ = 1; } \
} \
unsigned int __consEnter__() { \
	__consEnterC__ += 1; \
	return __consEnterC__ - 1; \
} \
void __removedone__( unsigned int ticket ) { \
	if ( ticket != __removedoneC__ ) { __BCHECK_PRT__( "**** BARGING ERROR **** consumer over consumer" ); } \
	if ( __next__ == 1 ) { __BCHECK_PRT__( "**** BARGING ERROR **** consumer over producer" ); } \
	__next__ = 0; \
	__removedoneC__ += 1; \
} \
void consSignal( uCondLock & c ) { \
	if ( ! c.empty() ) { __next__ = 2; } \
}

#define BCHECK_DECL \
	BCHECK( __BCHECK_DECL__ )
#define PROD_ENTER \
	BCHECK( unsigned int __barging_ticket__ = __prodEnter__(); )
#define INSERT_DONE \
	BCHECK( __insertdone__( __barging_ticket__ ); )
#define PROD_SIGNAL( cond ) \
	BCHECK( __prodSignal__( cond ); )
#define CONS_ENTER \
	BCHECK( unsigned int __barging_ticket__ = __consEnter__(); )
#define REMOVE_DONE \
	BCHECK( __removedone__( __barging_ticket__ ); )
#define CONS_SIGNAL( cond ) \
	BCHECK( consSignal( cond ); )
