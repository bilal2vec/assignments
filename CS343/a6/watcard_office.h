#ifndef WATCARD_OFFICE_H_
#define WATCARD_OFFICE_H_

#include <queue>
#include <vector>

#include "watcard.h"

using namespace std;

_Monitor Printer;
_Monitor Bank;

typedef struct Args {
  unsigned int s_id;
  unsigned int amount;
  WATCard *watcard;
  Args(unsigned int s_id, unsigned int amount, WATCard *watcard);
} Args;

_Task WATCardOffice {
  struct Job {                 // marshalled arguments and return future
    Args args;                 // call arguments
    WATCard::FWATCard result;  // return future
    Job(Args args);
  };

  // communicates with bank
  _Task Courier {
    unsigned int id;
    Printer & printer;
    Bank & bank;
    WATCardOffice *office;

    void main();

   public:
    Courier(unsigned int id, Printer &printer, Bank &bank,
            WATCardOffice *office);
  };

  Printer & printer;
  Bank & bank;
  unsigned int num_couriers;

  // queue/vector of job/watcard pointers instead of objects to easily pass them
  // to couriers inside Args
  Courier **couriers;
  queue<Job *> jobs;
  vector<WATCard *> watcards;

  void main();

 public:
  _Event Lost{};  // lost WATCard
  WATCardOffice(Printer & prt, Bank & bank, unsigned int numCouriers);
  ~WATCardOffice();
  WATCard::FWATCard create(unsigned int sid, unsigned int amount)
      __attribute__((warn_unused_result));
  WATCard::FWATCard transfer(unsigned int sid, unsigned int amount,
                             WATCard *card) __attribute__((warn_unused_result));
  Job *requestWork() __attribute__((warn_unused_result));
};

#endif  // WATCARD_OFFICE_H_