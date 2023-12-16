#include "printer.h"

#include <iostream>

using namespace std;

void Printer::flush_buffer() {
  unsigned int n_data = 6 + num_students + num_vending_machines + num_couriers;
  for (unsigned int i = 0; i < n_data; i++) {
    // Align each column to multiple of \t
    if (i > 0) {
      cout << "\t";
    }

    // If the state is empty, skip
    if (data[i].is_cleared) {
      continue;
    }

    // Print out the state char
    cout << data[i].state;

    // Print out state-specific data, if any
    if (data[i].value_1 != (unsigned int)-1) {
      cout << data[i].value_1;
    }

    if (data[i].value_2 != (unsigned int)-1) {
      cout << "," << data[i].value_2;
    }

    // Clear the state
    data[i].is_cleared = true;
    data[i].value_1 = (unsigned int)-1;
    data[i].value_2 = (unsigned int)-1;
  }

  cout << endl;
}

Printer::Printer(unsigned int numStudents, unsigned int numVendingMachines,
                 unsigned int numCouriers)
    : num_students(numStudents),
      num_vending_machines(numVendingMachines),
      num_couriers(numCouriers) {
  unsigned int n_data = 6 + numStudents + numVendingMachines + numCouriers;
  data = new DATA[n_data];

  for (unsigned int i = 0; i < n_data; i++) {
    data[i].is_cleared = true;
    data[i].value_1 = (unsigned int)-1;
    data[i].value_2 = (unsigned int)-1;
  }

  // Print out the header, line 1
  cout << "Parent\tGropoff\tWATOff\tNames\tTruck\tPlant";

  for (unsigned int i = 0; i < numStudents; i++) {
    cout << "\tStud" << i;
  }

  for (unsigned int i = 0; i < numVendingMachines; i++) {
    cout << "\tMach" << i;
  }

  for (unsigned int i = 0; i < numCouriers; i++) {
    cout << "\tCour" << i;
  }

  cout << endl;

  // Print out the header, line 2
  for (unsigned int i = 0; i < n_data; i++) {
    cout << "*******";
    cout << "\t";
  }

  cout << endl;
}

Printer::~Printer() {
  flush_buffer();
  delete[] data;
  cout << "***********************" << endl;
}

void Printer::print(Printer::Kind kind, char state) {
  if (!data[kind].is_cleared) {
    flush_buffer();
  }

  data[kind].is_cleared = false;
  data[kind].kind = kind;
  data[kind].state = state;
}

void Printer::print(Printer::Kind kind, char state, unsigned int value1) {
  print(kind, state);

  data[kind].value_1 = value1;
}

void Printer::print(Printer::Kind kind, char state, unsigned int value1,
                    unsigned int value2) {
  print(kind, state, value1);

  data[kind].value_2 = value2;
}

void Printer::print(Printer::Kind kind, unsigned int lid, char state) {
  // The first six elements are reserved for the first six kinds
  int offset = 6;

  // Add the offset for the kind {Student, Vending, Courier}
  if (kind == Student) {
    offset += lid;
  } else if (kind == Vending) {
    offset += num_students + lid;
  } else if (kind == Courier) {
    offset += num_students + num_vending_machines + lid;
  }

  if (!data[offset].is_cleared) {
    flush_buffer();
  }

  data[offset].is_cleared = false;

  data[offset].kind = kind;
  data[offset].state = state;
}

void Printer::print(Printer::Kind kind, unsigned int lid, char state,
                    unsigned int value1) {
  print(kind, lid, state);

  // The first six elements are reserved for the first six kinds
  int offset = 6;

  // Add the offset for the kind {Student, Vending, Courier}
  if (kind == Student) {
    offset += lid;
  } else if (kind == Vending) {
    offset += num_students + lid;
  } else if (kind == Courier) {
    offset += num_students + num_vending_machines + lid;
  }

  data[offset].value_1 = value1;
}

void Printer::print(Printer::Kind kind, unsigned int lid, char state,
                    unsigned int value1, unsigned int value2) {
  print(kind, lid, state, value1);

  // The first six elements are reserved for the first six kinds
  int offset = 6;

  // Add the offset for the kind {Student, Vending, Courier}
  if (kind == Student) {
    offset += lid;
  } else if (kind == Vending) {
    offset += num_students + lid;
  } else if (kind == Courier) {
    offset += num_students + num_vending_machines + lid;
  }

  data[offset].value_2 = value2;
}
