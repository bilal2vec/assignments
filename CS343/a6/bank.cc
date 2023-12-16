#include "bank.h"

Bank::Bank(unsigned int numStudents) : num_students(numStudents) {
  balances = new unsigned int[numStudents];

  // Initialize all balances to 0
  for (unsigned int i = 0; i < num_students; i++) {
    balances[i] = 0;
  }
}

Bank::~Bank() { delete[] balances; }

void Bank::deposit(unsigned int id, unsigned int amount) {
  balances[id] += amount;
}

void Bank::withdraw(unsigned int id, unsigned int amount) {
  // Block until the balance is greater than the amount to withdraw
  while (balances[id] < amount) {
    _Accept(deposit);
  }

  balances[id] -= amount;
}