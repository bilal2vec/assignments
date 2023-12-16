#ifndef GROUPOFF_H
#define GROUPOFF_H

#include <vector>

#include "watcard.h"

using namespace std;

_Monitor Printer;

_Task Groupoff {
  Printer & printer;
  unsigned int num_students;
  unsigned int soda_cost;
  unsigned int groupoff_delay;
  vector<WATCard *> watcards;
  vector<WATCard::FWATCard> gift_cards;

  void main();

 public:
  Groupoff(Printer & prt, unsigned int numStudents, unsigned int sodaCost,
           unsigned int groupoffDelay);
  ~Groupoff();
  WATCard::FWATCard giftCard();
};

#endif  // GROUPOFF_H