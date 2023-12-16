#include "student.h"

#include "bottling_plant.h"
#include "groupoff.h"
#include "name_server.h"
#include "printer.h"
#include "vending_machine.h"
#include "watcard_office.h"

void Student::main() {
  unsigned int num_bottles = prng(1, max_purchases);
  unsigned int favourite_flavour = prng(0, 3);

  printer.print(Printer::Kind::Student, id, 'S', favourite_flavour,
                num_bottles);

  WATCard::FWATCard watcard_future = watcard_office.create(id, 5);
  WATCard::FWATCard giftcard_future = groupoff.giftCard();
  VendingMachine* vending_machine = name_server.getMachine(id);

  printer.print(Printer::Kind::Student, id, 'V', vending_machine->getId());

  for (unsigned int i = 0; i < num_bottles; i++) {
    yield(prng(1, 10));

    // Attempt to buy a soda with a watcard or giftcard, prioritizing giftcard
    while (true) {
      // need to be accessible in try and catch
      WATCard* watcard = nullptr;

      // note giftcards are always loaded with enough money to buy a soda, don't
      // need to check funds case
      _Select(giftcard_future) {
        // get the giftcard value, try to buy, print "G" (bought soda with
        // giftcard successfully), and reset the giftcard
        try {
          watcard = giftcard_future();
          BottlingPlant::Flavours fav_flavor =
              static_cast<BottlingPlant::Flavours>(favourite_flavour);
          vending_machine->buy(fav_flavor, *watcard);
          giftcard_future.reset();
          printer.print(Printer::Kind::Student, id, 'G', favourite_flavour,
                        watcard->getBalance());
          break;
        }
        // If vending machine gives you a free soda, yield 4, and print "a"
        // (free soda with giftcard)
        catch (VendingMachine::Free& e) {
          if (prng(0, 1) == 0) {
            yield(4);
          } else {
            printer.print(Printer::Kind::Student, id, 'X');
          }
          printer.print(Printer::Kind::Student, id, 'a', favourite_flavour,
                        watcard->getBalance());
          break;
        }
        // If vending machine is out of stock, get a new vending machine and
        // try again: no break
        catch (VendingMachine::Stock& e) {
          vending_machine = name_server.getMachine(id);
          printer.print(Printer::Kind::Student, id, 'V', favourite_flavour,
                        vending_machine->getId());
        }
      }
      or _Select(watcard_future) {
        try {
          // get the watcard value, try to buy, print "B" (bought soda with
          // watcard successfully)
          watcard = watcard_future();

          BottlingPlant::Flavours fav_flavor =
              static_cast<BottlingPlant::Flavours>(favourite_flavour);
          vending_machine->buy(fav_flavor, *watcard);
          printer.print(Printer::Kind::Student, id, 'B', favourite_flavour,
                        watcard->getBalance());
          break;
        }
        // lost watcard, get a new one and try again: no break
        catch (WATCardOffice::Lost& e) {
          printer.print(Printer::Kind::Student, id, 'L');
          watcard_future = watcard_office.create(id, 5);
        }
        // Not enough funds, add $5 and try again: no break and no printing!
        catch (VendingMachine::Funds& e) {
          watcard_future =
              watcard_office.transfer(id, vending_machine->cost() + 5, watcard);
        }
        // If vending machine gives you a free soda, yield 4, and print "A"
        // (free soda with watcard)
        catch (VendingMachine::Free& e) {
          if (prng(0, 1) == 0) {
            yield(4);
          } else {
            printer.print(Printer::Kind::Student, id, 'X');
          }
          printer.print(Printer::Kind::Student, id, 'A', favourite_flavour,
                        watcard->getBalance());
          break;
        }
        // If vending machine is out of stock, get a new vending machine and
        // try again: no break
        catch (VendingMachine::Stock& e) {
          vending_machine = name_server.getMachine(id);
          printer.print(Printer::Kind::Student, id, 'V', favourite_flavour,
                        vending_machine->getId());
        }
      }
    }
  }

  printer.print(Printer::Kind::Student, id, 'F');
}

Student::Student(Printer& prt, NameServer& nameServer,
                 WATCardOffice& cardOffice, Groupoff& groupoff, unsigned int id,
                 unsigned int maxPurchases)
    : printer(prt),
      name_server(nameServer),
      watcard_office(cardOffice),
      groupoff(groupoff),
      id(id),
      max_purchases(maxPurchases) {}