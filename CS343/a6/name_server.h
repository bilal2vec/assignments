#ifndef NAME_SERVER_H_
#define NAME_SERVER_H_

_Monitor Printer;
_Task VendingMachine;

_Task NameServer {
  Printer & printer;
  unsigned int num_vending_machines;
  unsigned int num_students;

  unsigned int *student_machine_map;
  VendingMachine **vending_machines;
  unsigned int n_vending_machines_registered;

  void main();

 public:
  NameServer(Printer & prt, unsigned int numVendingMachines,
             unsigned int numStudents);
  ~NameServer();
  void VMregister(VendingMachine * vendingmachine);
  VendingMachine *getMachine(unsigned int id)
      __attribute__((warn_unused_result));
  VendingMachine **getMachineList() __attribute__((warn_unused_result));
};

#endif  // NAME_SERVER_H_