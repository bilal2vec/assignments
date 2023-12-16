#ifndef Q2TALLYVOTES_H_
#define Q2TALLYVOTES_H_

#ifdef NOOUTPUT
#define PRINT(stmt)
#else
#define PRINT(stmt) stmt
#endif  // NOOUTPUT

_Monitor Printer;

#if defined(EXT)  // external scheduling monitor solution
_Monitor TallyVotes {
#elif defined(INT)   // internal scheduling monitor solution
_Monitor TallyVotes {
  uCondition bench;
#elif defined(INTB)  // internal scheduling monitor solution with barging
_Monitor TallyVotes {
  int current_ticket;
  int n_barging_tasks;
  uCondition
      bench;    // only one condition variable (variable name may be changed)
  void wait();  // barging version of wait
  void signalAll();  // unblock all waiting tasks
#elif defined(AUTO)  // automatic-signal monitor solution
#include "AutomaticSignal.h"

_Monitor TallyVotes {
  AUTOMATIC_SIGNAL;
#elif defined(TASK)  // internal/external scheduling task solution
_Task TallyVotes {
  uCondition bench;
  void main();
#else
#error unsupported voter type
#endif
  unsigned int n_voters;
  unsigned int group_size;
  Printer & printer;

  int turnstile_count;
  int current_group_number;

  int n_picture_votes;
  int n_statue_votes;
  int n_giftshop_votes;

 public:
  _Event Failed{};
  struct Ballot {
    unsigned int picture, statue, giftshop;
  };
  enum TourKind : char { Picture = 'p', Statue = 's', GiftShop = 'g' };
  struct Tour {
    TourKind tourkind;
    unsigned int groupno;
  };

  TallyVotes(unsigned int voters, unsigned int group, Printer& printer);
  Tour vote(unsigned int id, Ballot ballot);
  void done(
#if defined(TASK)  // task solution
      unsigned int id
#endif
  );
 private:
  Tour current_tour;
#if defined(TASK)
  unsigned int _id;
  Ballot _ballot;
#endif
};

#endif  // Q2TALLYVOTES_H_