void CriticalSection() {
	static uBaseTask * curr;							// shared
	curr = &uThisTask();
	for ( unsigned int i = 0; i < 100; i += 1 ) {		// work
		if ( curr != &uThisTask() ) { abort( "Interference" ); } // check
	}
}

enum Intent { WantIn, DontWantIn } * Last;				// shared

_Task Dekker {
	Intent & me, & you;

	void main() {
		for ( unsigned int i = 0; i < 10'000'000; i += 1 ) {
			for ( ;; ) {								// entry protocol, high priority
				me = WantIn;
				__asm__ __volatile__( "mfence" );		// prevent hardware reordering (x86)
			  if ( you == DontWantIn ) break;
				if ( ::Last == &me ) {
					me = DontWantIn;
					while ( ::Last == &me ) {}
				}
			}
			CriticalSection();							// critical section
			::Last = &me;								// exit protocol
			me = DontWantIn;
		}
	}
  public:
	Dekker( Intent & me, Intent & you ) : me(me), you(you) {}
};

int main() {
	uProcessor p;
	Intent me = DontWantIn, you = DontWantIn;			// shared
	::Last = &me;										// arbitrary who starts as last
	Dekker t0( me, you ), t1( you, me );
}