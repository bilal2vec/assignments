#include <iostream>

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
		#define __BCHECK_PRT__( msg ) std::osacquire( std::cerr ) << msg << std::endl
	#endif
#else
	#define BCHECK( stmt )
#endif // BARGINGCHECK

#define __BCHECK_DECL__ \
unsigned int __groupTick__ = 0, __exit_group_tick__ = 0, __exit_call_count__ = 0, __callCnt__ = 0; \
\
unsigned int __prodEnter__( unsigned int group ) { \
	__callCnt__ += 1; \
	if ( __callCnt__ % group == 0 ) return __groupTick__++; \
	return __groupTick__; \
} \
void __prodleave__( unsigned int ticket, unsigned int group ) { \
	__exit_call_count__ += 1; \
	if ( ticket != __exit_group_tick__ ) { __BCHECK_PRT__( "**** BARGING ERROR **** voter over voter" ); } \
	if ( __exit_call_count__ % group == 0 ) __exit_group_tick__ += 1; \
}

#define BCHECK_DECL \
	BCHECK( __BCHECK_DECL__ )
#define VOTER_ENTER( group ) \
	BCHECK( unsigned int __voter_ticket__ = __prodEnter__( group ); )
#define VOTER_LEAVE( group ) \
	BCHECK( __prodleave__( __voter_ticket__, group ); )
