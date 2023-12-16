#include "q3voter.h"

#include "q3printer.h"

Voter::Voter( unsigned int id, unsigned int nvotes, TallyVotes & voteTallier, Printer & printer ) : id(id), n_votes(nvotes), vote_tallier(voteTallier), printer(printer) {}

void Voter::main() {
  yield(prng(19));
  
  try {
    _Enable {
      for (unsigned int i = 0; i < n_votes; i++) {
        PRINT(printer.print(id, States::Start));

        yield(prng(4));

        TallyVotes::Tour tour = vote_tallier.vote(id, cast());

        yield(prng(4));

        PRINT(printer.print(id, States::Going, tour));
      }
    }
  } catch (TallyVotes::Failed &e) {
    PRINT(printer.print(id, States::Failed));
  } 

  #if defined(MC) || defined(BAR)
  vote_tallier.done(id);
  #endif

  #if defined(SEM)
  vote_tallier.done();
  #endif
 
  PRINT(printer.print(id, States::Terminated));

}