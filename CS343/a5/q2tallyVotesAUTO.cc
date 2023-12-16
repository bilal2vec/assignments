#include "q2tallyVotes.h"

// empty comment for autoformatter to maintain this import ordering
#include "q2_printer.h"

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
  // If quorum failure, throw
  if (n_voters < group_size) {
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
    PRINT(printer.print(id, Voter::States::Complete, current_tour));

    // Reset vote counts
    n_picture_votes = 0;
    n_statue_votes = 0;
    n_giftshop_votes = 0;

    // Increment group number
    current_group_number++;

    // Decrement turnstile count
    turnstile_count--;
  } else {
    // Wait here until turnstile count is equal to group size minus 1 (i.e. the
    // last voter has voted and exited decrementing turnstile count), print the
    // blocked message before waiting and unblock message after waiting
    WAITUNTIL(
        turnstile_count == group_size - 1,
        PRINT(printer.print(id, Voter::States::Block, turnstile_count)),
        PRINT(printer.print(id, Voter::States::Unblock, --turnstile_count)));

    if (n_voters < group_size) {
      _Throw Failed();
    }
  }

  EXIT();

  // Return tour
  return current_tour;
}

void TallyVotes::done() {
  // Decrement number of voters
  n_voters--;

  EXIT();
}