#include "name_server.h"

#include "printer.h"
#include "vending_machine.h"

void NameServer::main() {
  printer.print(Printer::Kind::NameServer, 'S');

  // First, register all vending machines
  for (unsigned int i = 0; i < num_vending_machines; i++) {
    // Exit if destructor called early
    _Accept(~NameServer) {
      printer.print(Printer::Kind::NameServer, 'F');

      return;
    }
    or _Accept(VMregister) {}
  }

  while (true) {
    // Exit if destructor called
    _Accept(~NameServer) { break; }
    // Allow students/truck to get vending machines
    or _Accept(getMachine){} or _Accept(getMachineList) {}
  }

  printer.print(Printer::Kind::NameServer, 'F');
}

NameServer::NameServer(Printer& prt, unsigned int numVendingMachines,
                       unsigned int numStudents)
    : printer(prt),
      num_vending_machines(numVendingMachines),
      num_students(numStudents),
      n_vending_machines_registered(0) {
  student_machine_map = new unsigned int[num_students];
  vending_machines = new VendingMachine*[num_vending_machines];

  for (unsigned int i = 0; i < num_vending_machines; i++) {
    vending_machines[i] = nullptr;
  }

  for (unsigned int i = 0; i < num_students; i++) {
    student_machine_map[i] = i % num_vending_machines;
  }
}

NameServer::~NameServer() {
  delete[] student_machine_map;
  delete[] vending_machines;
}

void NameServer::VMregister(VendingMachine* vendingmachine) {
  printer.print(Printer::Kind::NameServer, 'R', vendingmachine->getId());
  vending_machines[n_vending_machines_registered++] = vendingmachine;
}

VendingMachine* NameServer::getMachine(unsigned int id) {
  unsigned int machine_idx = student_machine_map[id];
  printer.print(Printer::Kind::NameServer, 'N', id, machine_idx);
  student_machine_map[id] = (machine_idx + 1) % num_vending_machines;
  return vending_machines[machine_idx];
}

VendingMachine** NameServer::getMachineList() { return vending_machines; }