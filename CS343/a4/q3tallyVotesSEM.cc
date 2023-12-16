#include "q3tallyVotes.h"

#include "q3printer.h"
#include "q3voter.h"

TallyVotes::TallyVotes( unsigned int voters, unsigned int group, Printer & printer ) : mutex(1), turnstile(0), turnstile_count(0), voters(voters), group(group), printer(printer) {
  current_group_number = 1;

  n_picture_votes = 0;
  n_statue_votes = 0;
  n_giftshop_votes = 0;
}

TallyVotes::Tour TallyVotes::vote( unsigned int id, Ballot ballot ) {
  // Automatically throw if quorum failures
  // incase we hit a quorum failure for some voters before they ever grab a mutex
  if (voters < group) {
    _Throw Failed();
  }

  mutex.P();
  VOTER_ENTER(group);

  // If voter attempts to vote when we hit a quorum failure, throw
  if (voters < group) {
    _Throw Failed();
  }

  // Contribute to voting
  n_picture_votes += ballot.picture;
  n_statue_votes += ballot.statue;
  n_giftshop_votes += ballot.giftshop;

  // Print voting
  PRINT(printer.print(id, Voter::States::Vote, ballot));

  // Increment number of voters waiting for group to fill up
  turnstile_count++;

  if ((unsigned int)turnstile_count == group) {

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

    // Print tour
    PRINT(printer.print(id, Voter::States::Complete, current_tour));

    // Reset vote counts
    n_picture_votes = 0;
    n_statue_votes = 0;
    n_giftshop_votes = 0;

    // Increment group number
    current_group_number++;

    // Decrement turnstile 2 count
    turnstile_count--;
  } else {
    // Print voter blocked
    PRINT(printer.print(id, Voter::States::Block, turnstile_count));

    // Each voter will let another voter in until the group is full
    // P/Wait at turnstile, V/release mutex
    // Let another voter come in from the mutex
    VOTER_LEAVE(group);
    turnstile.P(mutex);

    // If voter attempts to vote when we hit a quorum failure, throw
    if (voters < group) {
      _Throw Failed();
    }

    // NOW: Group size reached, removing blocked voters from turnstile one at a time
    turnstile_count--;

    // Print voter unblocked
    PRINT(printer.print(id, Voter::States::Unblock, turnstile_count));
  }

  // If voter attempts to vote when we hit a quorum failure, throw
  if (voters < group) {
    _Throw Failed();
  }

  // V/signal turnstile to let next voter in the tour group to leave to go on the tour
  // Barging pervention as we hold the lock between releasing and unblocking the next voter aka V/signal on the semaphore to unblock the next voter
  if (turnstile_count > 0) {
    turnstile.V();
  } else {
    // All voters in the group have left to go on the tour
    // V/release mutex to let next voter in to form a new group
    VOTER_LEAVE(group);
    mutex.V();
  }

  // Return tour
  return current_tour;
}

void TallyVotes::done() {
  mutex.P();

  voters--;

  if (voters < group) {
    turnstile.V();
    mutex.V();
  }

  mutex.V();
}