#ifndef _printer_h
#define _printer_h

#include "q3tallyVotes.h"
#include "q3voter.h"

typedef struct {
	int n;
} BLOCK_DATA;

typedef struct {
	int n;
	int gn;
} BARGING_DATA;

typedef struct {
	bool is_cleared;
	Voter::States state;
	union {
		TallyVotes::Tour tour;
		TallyVotes::Ballot ballot;
		BLOCK_DATA block_data;
		BARGING_DATA barging_data;
	} data;
} VoterStruct;

_Monitor Printer {
	unsigned int voters;
	VoterStruct *voter_structs;

	void flush_buffer();
public:
	Printer( unsigned int voters );
	~Printer();
	void print( unsigned int id, Voter::States state );
	void print( unsigned int id, Voter::States state, TallyVotes::Tour tour );
	void print( unsigned int id, Voter::States state, TallyVotes::Ballot vote );
	void print( unsigned int id, Voter::States state, unsigned int numBlocked );
	void print( unsigned int id, Voter::States state, unsigned int numBlocked, unsigned int group );
};

#endif /* printer.h */