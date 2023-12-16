#ifndef _q3tallyVotes_h
#define _q3tallyVotes_h


#ifdef NOOUTPUT
#define PRINT( stmt )
#else
#define PRINT( stmt ) stmt
#endif // NOOUTPUT

_Monitor Printer;

#if defined(MC)
#include "BargingCheckVote.h"
class TallyVotes {
	uOwnerLock mutex;
	uCondLock turnstile_1;
	uCondLock turnstile_2;
	bool signalling_flag;
	int turnstile_1_count;
	int turnstile_2_count;
	BCHECK_DECL;
#elif defined(SEM)				// semaphore solution
#include "BargingCheckVote.h"

#include <uSemaphore.h>

class TallyVotes {
	uSemaphore mutex;
	uSemaphore turnstile;
	int turnstile_count;
	BCHECK_DECL;
#elif defined(BAR)				// barrier solution
#include "uBarrier.h"

_Cormonitor TallyVotes : public uBarrier {
	void last();
	int count;
#else
	#error unsupported voter type
#endif
	unsigned int voters;
	unsigned int group;
	Printer & printer;

	int current_group_number;

	int n_picture_votes;
	int n_statue_votes;
	int n_giftshop_votes;

public:
	_Event Failed {};
	struct Ballot { unsigned int picture, statue, giftshop; };
	enum TourKind : char { Picture = 'p', Statue = 's', GiftShop = 'g' };
	struct Tour { TourKind tourkind; unsigned int groupno; };

	TallyVotes( unsigned int voters, unsigned int group, Printer & printer );
	Tour vote( unsigned int id, Ballot ballot );
	void done(
		#if defined(MC) || defined(BAR)
		unsigned int id
		#endif
	);
private:
	Tour current_tour;
};


#endif /* q3tallyVotes.h */