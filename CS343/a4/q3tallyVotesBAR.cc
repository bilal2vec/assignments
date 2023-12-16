#include "q3tallyVotes.h"

#include "q3printer.h"
#include "q3voter.h"

TallyVotes::TallyVotes( unsigned int voters, unsigned int group, Printer & printer ) : uBarrier(group), count(0), voters(voters), group(group), printer(printer) {
  current_group_number = 1;

  n_picture_votes = 0;
  n_statue_votes = 0;
  n_giftshop_votes = 0;
}

// the group-th call to uBarrier::block() runs last() to decide which tour to take
void TallyVotes::last() {
  // Choose the tour kind, break ties by priority in order (high to low) giftshop -> picture -> statue
  if (n_giftshop_votes >= n_picture_votes && n_giftshop_votes >= n_statue_votes) {
    current_tour.tourkind = TourKind::GiftShop;
  } else if (n_picture_votes >= n_giftshop_votes && n_picture_votes >= n_statue_votes) {
    current_tour.tourkind = TourKind::Picture;
  } else {
    current_tour.tourkind = TourKind::Statue;
  }

  // Set group number
  current_tour.groupno = current_group_number;

  // Reset vote counts
  n_picture_votes = 0;
  n_statue_votes = 0;
  n_giftshop_votes = 0;

  // Increment group number
  current_group_number++;

}

TallyVotes::Tour TallyVotes::vote( unsigned int id, Ballot ballot ) {
  // If voter attempts to vote when we hit a quorum failure, throw
  if (voters < group) {
    _Throw Failed();
  }

  // Contribute to voting
  n_picture_votes += ballot.picture;
  n_statue_votes += ballot.statue;
  n_giftshop_votes += ballot.giftshop;

  // Increment number of voters waiting for group to fill up
  count++;

  // Print voting
  PRINT(printer.print(id, Voter::States::Vote, ballot));

  if ((unsigned int) count == group) {
    // Call block the group-th time to decide which tour to take in last()
    uBarrier::block();

    // Decrement count
    count--;

    // Print tour
    PRINT(printer.print(id, Voter::States::Complete, current_tour));

  } else {
    // Print voter blocked
    PRINT(printer.print(id, Voter::States::Block, count));

    // uBarrier::block() will block until the group-th call to uBarrier::block()
    uBarrier::block();

    // NOW: Group size reached, removing blocked voters one at a time
    count--;

    // If voter attempts to vote when we hit a quorum failure, throw
    if (voters < group) {
      _Throw Failed();
    }

    // Print voter unblocked
    PRINT(printer.print(id, Voter::States::Unblock, count));
  }

  // If voter attempts to vote when we hit a quorum failure, throw
  if (voters < group) {
    _Throw Failed();
  }

  return current_tour;
}

void TallyVotes::done(unsigned int id) {
  // Decrement number of voters
  voters--;

  // If quorum failure
  if (voters < group) {
    // If we have some voters left, call uBarrier::block() so we can hit a last() for the last time
    if (count > 0) {
      uBarrier::block();
    }
  }
}