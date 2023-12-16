#ifndef BANK_H_
#define BANK_H_

_Monitor Bank {
  unsigned int num_students;
  unsigned int *balances;

 public:
  Bank(unsigned int numStudents);
  ~Bank();
  void deposit(unsigned int id, unsigned int amount);
  void withdraw(unsigned int id, unsigned int amount);
};

#endif  // BANK_H_