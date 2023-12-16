#ifndef PARENT_H_
#define PARENT_H_

_Monitor Printer;
_Monitor Bank;

_Task Parent {
  Printer & printer;
  Bank & bank;

  unsigned int num_students;
  unsigned int parental_delay;

  void main();

 public:
  Parent(Printer & prt, Bank & bank, unsigned int numStudents,
         unsigned int parentalDelay);
};

#endif  // PARENT_H_