#include "vending_machine.h"

#include "name_server.h"
#include "printer.h"
#include "watcard.h"

void VendingMachine::main() {
  printer.print(Printer::Kind::Vending, id, 'S', soda_cost);

  while (true) {
    // Exit if destructor called
    _Accept(~VendingMachine) { break; }
    // A call to inventory starts the restocking process, stop calls to buy
    or _Accept(inventory) {
      allow_buy = false;
      printer.print(Printer::Kind::Vending, id, 'r');
    }
    // A call to restocked ends the restocking process
    or _Accept(restocked) {
      allow_buy = true;
      printer.print(Printer::Kind::Vending, id, 'R');
    }
    // Block calls to buy until restocked
    or _When(allow_buy) _Accept(buy) {
      // OUt of stock error
      if (n_inventory[flavor] == 0) {
        exception_type = ExceptionType::Stock;
      }
      // Free soda
      else if (prng(4) == 0) {
        n_inventory[flavor]--;
        printer.print(Printer::Kind::Vending, id, 'A', flavor,
                      n_inventory[flavor]);
        exception_type = ExceptionType::Free;
        // Insufficient funds
      } else if (watcard->getBalance() < soda_cost) {
        exception_type = ExceptionType::Funds;
        // Successful purchase
      } else {
        n_inventory[flavor]--;
        exception_type = ExceptionType::None;
        watcard->withdraw(soda_cost);
        printer.print(Printer::Kind::Vending, id, 'B', flavor,
                      n_inventory[flavor]);
      }

      // signal block on bench to wake up and directly go to buy()
      bench.signalBlock();
    }
  }

  printer.print(Printer::Kind::Vending, id, 'F');
}

VendingMachine::VendingMachine(Printer& prt, NameServer& nameServer,
                               unsigned int id, unsigned int sodaCost)
    : printer(prt),
      name_server(nameServer),
      id(id),
      soda_cost(sodaCost),
      allow_buy(true) {
  // Initialize to 0
  for (unsigned int i = 0; i < 4; i++) {
    n_inventory[i] = 0;
  }

  // Register with name server
  name_server.VMregister(this);
}

void VendingMachine::buy(BottlingPlant::Flavours flavour, WATCard& card) {
  // store args in member variables
  flavor = static_cast<unsigned int>(flavour);
  watcard = &card;

  // wait on bench until restocked
  bench.wait();

  // throw exception if applicable
  if (exception_type == ExceptionType::None) {
    return;
  } else if (exception_type == ExceptionType::Stock) {
    _Throw Stock();
  } else if (exception_type == ExceptionType::Funds) {
    _Throw Funds();
  } else if (exception_type == ExceptionType::Free) {
    _Throw Free();
  }
}

unsigned int* VendingMachine::inventory() { return n_inventory; }

// doesn't need to do anything itself, it just triggers the _accept in main()
void VendingMachine::restocked() {}

_Nomutex unsigned int VendingMachine::cost() const { return soda_cost; }

_Nomutex unsigned int VendingMachine::getId() const { return id; }
