#include "q2tallyVotes.h"

// empty comment for autoformatter to maintain this import ordering
#include "q2_printer.h"

void TallyVotes::main() {
  while (true) {
    try {
      // Implicitly stop task if destructor is called
      _Accept(~TallyVotes) { break; }
      // When a voter is calls vote...
      or _Accept(vote) {
        // If quorum failure, throw
        if (n_voters < group_size) {
          _Throw Failed();
        }

        // Contribute to voting
        n_picture_votes += _ballot.picture;
        n_statue_votes += _ballot.statue;
        n_giftshop_votes += _ballot.giftshop;

        // Print voting
        PRINT(printer.print(_id, Voter::States::Vote, _ballot));

        // Increment number of voters waiting for group to fill up
        turnstile_count++;

        // Print voter is blocked
        PRINT(printer.print(_id, Voter::States::Block, turnstile_count));

        if ((unsigned int)turnstile_count == group_size) {
          // Choose the tour kind, break ties by priority in order (high to low)
          // giftshop -> picture -> statue
          if (n_giftshop_votes >= n_picture_votes &&
              n_giftshop_votes >= n_statue_votes) {
            current_tour.tourkind = TourKind::GiftShop;
          } else if (n_picture_votes >= n_giftshop_votes &&
                     n_picture_votes >= n_statue_votes) {
            current_tour.tourkind = TourKind::Picture;
          } else {
            current_tour.tourkind = TourKind::Statue;
          }

          // Set group number
          current_tour.groupno = current_group_number;

          // Print tour
          PRINT(printer.print(_id, Voter::States::Complete, current_tour));

          // Reset vote counts
          n_picture_votes = 0;
          n_statue_votes = 0;
          n_giftshop_votes = 0;

          // Increment group number
          current_group_number++;

          // Once the nth voter has voted, unblock all voters one at a time so
          // they can leave on tour.
          while (!bench.empty()) {
            // Decrement number of voters waiting for group to fill up
            turnstile_count--;

            PRINT(printer.print(bench.front(), Voter::States::Unblock,
                                turnstile_count));

            // Using signalBlock() so that control transfers to each voter in
            // order of arrival at the wait() call so they can return
            bench.signalBlock();
          }
        }
      }
      // Case that a voter calls done
      or _Accept(done) {
        // Print done, TASK-specific
        PRINT(printer.print(_id, Voter::States::Done));

        // If quorum failure, unblock all voters one at a time and let them
        // throw
        if (n_voters < group_size) {
          while (!bench.empty()) {
            // Decrement number of voters waiting for group to fill up
            turnstile_count--;

            // Print voter unblocked
            PRINT(printer.print(bench.front(), Voter::States::Unblock,
                                turnstile_count));

            // Using signalBlock() so that control transfers to each voter in
            // order of arrival at the wait() call so they can return
            bench.signalBlock();
          }
        }
      }
      // Catch a quorum failure thrown by a voter in vote()
    } catch (uMutexFailure::RendezvousFailure& e) {
      while (!bench.empty()) {
        // Decrement number of voters waiting for group to fill up
        turnstile_count--;

        // Print voter unblocked
        PRINT(printer.print(bench.front(), Voter::States::Unblock,
                            turnstile_count));

        // Using signalBlock() so that control transfers to each voter in
        // order of arrival at the wait() call so they can return
        bench.signalBlock();
      }
    }
  }
}

TallyVotes::TallyVotes(unsigned int voters, unsigned int group,
                       Printer& printer)
    : n_voters(voters), group_size(group), printer(printer) {
  turnstile_count = 0;

  current_group_number = 1;

  n_picture_votes = 0;
  n_statue_votes = 0;
  n_giftshop_votes = 0;
}

TallyVotes::Tour TallyVotes::vote(unsigned int id, Ballot ballot) {
  // Set id and ballot class variables
  _id = id;
  _ballot = ballot;

  // If quorum failure, throw
  if (n_voters < group_size) {
    _Throw Failed();
  }

  // Contribute to voting, wait for group to fill up
  bench.wait(_id);

  // If quorum failure, throw
  if (n_voters < group_size) {
    _Throw Failed();
  }

  // Return tour
  return current_tour;
}

void TallyVotes::done(unsigned int id) {
  // Decrement number of voters
  n_voters--;
}