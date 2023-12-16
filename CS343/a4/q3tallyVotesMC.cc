#include "q3tallyVotes.h"

#include "q3printer.h"
#include "q3voter.h"

TallyVotes::TallyVotes( unsigned int voters, unsigned int group, Printer & printer ) : voters(voters), group(group), printer(printer) {
  signalling_flag = false;
  turnstile_1_count = 0;
  turnstile_2_count = 0;

  current_group_number = 1;

  n_picture_votes = 0;
  n_statue_votes = 0;
  n_giftshop_votes = 0;
}

TallyVotes::Tour TallyVotes::vote( unsigned int id, Ballot ballot ) {
  // If voter attempts to vote when we hit a quorum failure, throw
  if (voters < group) {
    _Throw Failed();
  }

  // Acquire mutex
  mutex.acquire();
  VOTER_ENTER(group);

  // If voter attempts to vote when we hit a quorum failure, throw
  if (voters < group) {
    _Throw Failed();
  }

  // signalling flag is used to tell you to wait at turnstile 1
  if (signalling_flag) {
    // Increment turnstile 1 count
    turnstile_1_count++;

    // Print voter is barging
    PRINT(printer.print(id, Voter::States::Barging, turnstile_1_count, current_group_number));

    // Wait at turnstile
    turnstile_1.wait(mutex);

    // Decrement turnstile 1 count
    turnstile_1_count--;
  }

  // Set signalling flag to true so that the next voter will wait at turnstile 1
  signalling_flag = true;

  // Contribute to voting
  n_picture_votes += ballot.picture;
  n_statue_votes += ballot.statue;
  n_giftshop_votes += ballot.giftshop;

  // Print voting
  PRINT(printer.print(id, Voter::States::Vote, ballot));

  // Increment number of voters waiting for group to fill up
  turnstile_2_count++;

  // If we have enough voters to form a group
  if ((unsigned int)turnstile_2_count == group) {

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
    turnstile_2_count--;
  } else {
    // Print voter blocked
    PRINT(printer.print(id, Voter::States::Block, turnstile_2_count));

    // Each voter will let another voter in until the group is full
    // Let another voter in from turnstile 1 to vote
    // If there are no current voters at turnstile 1, then reset the signal
    // so that the next voter will come in and wait at turnstile 2
    if (!turnstile_1.signal()) {
      signalling_flag = false;
    }

    // Wait for group to fill up
    turnstile_2.wait(mutex);

    // NOW: Group size reached, removing blocked voters from turnstile 2 one at a time
    turnstile_2_count--;

    // Print voter unblocked
    PRINT(printer.print(id, Voter::States::Unblock, turnstile_2_count));
  }

  // Signal turnstile 2 to let the next voter in the tour group leave to go on the tour
  if (!turnstile_2.signal()) {
    // If there are no current voters at turnstile 2, then all voters in the tour have left
    // Now let in a voter from turnstile 1 to vote
    // If there are no current voters at turnstile 1, then reset the signal so when the next voter comes in, they will wait at turnstile 2
    if (!turnstile_1.signal()) {
      signalling_flag = false;
    }
  }

  // Release mutex
  VOTER_LEAVE(group);
  mutex.release();


  return current_tour;
}

void TallyVotes::done(unsigned int id) {
  mutex.acquire();

  // Decrement number of voters left
  voters--;

  // Unblock waiting voters in case of quorum failure
  if (voters < group) {
    turnstile_1.signal();
    turnstile_2.signal();
  }

  mutex.release();
}