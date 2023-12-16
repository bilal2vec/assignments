#include "parent.h"

#include "bank.h"
#include "printer.h"

void Parent::main() {
  printer.print(Printer::Kind::Parent, 'S');

  while (true) {
    // Exit if destructor called
    _Accept(~Parent) { break; }
    _Else {
      // Yield
      yield(parental_delay);

      // Deposit a random amount of money to a random student
      unsigned int amount = prng(1, 3);
      unsigned int student = prng(0, num_students - 1);

      bank.deposit(student, amount);
      printer.print(Printer::Kind::Parent, 'D', student, amount);
    }
  }

  printer.print(Printer::Kind::Parent, 'F');
}

Parent::Parent(Printer& prt, Bank& bank, unsigned int numStudents,
               unsigned int parentalDelay)
    : printer(prt),
      bank(bank),
      num_students(numStudents),
      parental_delay(parentalDelay) {}