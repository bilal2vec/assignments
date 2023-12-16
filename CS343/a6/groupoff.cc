#include "groupoff.h"

#include "printer.h"

void Groupoff::main() {
  printer.print(Printer::Kind::Groupoff, 'S');

  // Accept one call to giftCard from each student
  for (unsigned int i = 0; i < num_students; i++) {
    _Accept(~Groupoff) { break; }
    or _Accept(giftCard);
  }

  // Give out gift cards until there are none left. The number of gift cards is
  // fixed at the statr
  while (!(gift_cards.empty())) {
    // Exit if destructor called
    _Accept(~Groupoff) { break; }
    _Else {
      yield(groupoff_delay);

      // Randomly choose a gift card to give out
      unsigned int gift_card_index = prng(gift_cards.size() - 1);

      // Create a new watcard
      WATCard* watcard = new WATCard();

      // Store the watcard in vector so we can keep the ptr available to put in
      // the giftcard future
      watcards.push_back(watcard);

      // Deposit the soda cost into the watcard
      watcard->deposit(soda_cost);

      // Put the watcard into the giftcard future
      printer.print(Printer::Kind::Groupoff, 'D', soda_cost);
      gift_cards[gift_card_index].delivery(watcard);

      // Remove the gift card from the vector of gift card futures
      gift_cards.erase(gift_cards.begin() + gift_card_index);

      break;
    }
  }

  printer.print(Printer::Kind::Groupoff, 'F');
}

Groupoff::Groupoff(Printer& prt, unsigned int numStudents,
                   unsigned int sodaCost, unsigned int groupoffDelay)
    : printer(prt),
      num_students(numStudents),
      soda_cost(sodaCost),
      groupoff_delay(groupoffDelay) {}

Groupoff::~Groupoff() {
  // Delete all watcard ptrs
  for (unsigned int i = 0; i < watcards.size(); i++) {
    delete watcards[i];
  }
}

WATCard::FWATCard Groupoff::giftCard() {
  // Create a new watcard future, store it since it needs to be accessible
  // later, and return it
  WATCard::FWATCard watcard_future;
  gift_cards.push_back(watcard_future);
  return watcard_future;
}