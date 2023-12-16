#include <uPRNG.h>

#include <iostream>
#include <string>

#include "bank.h"
#include "bottling_plant.h"
#include "config.h"
#include "groupoff.h"
#include "name_server.h"
#include "parent.h"
#include "printer.h"
#include "student.h"
#include "vending_machine.h"
#include "watcard_office.h"

using namespace std;

int main(int argc, char* argv[]) {
  struct cmd_error {};

  string config_file = "soda.config";
  int seed = -1;
  int n_processors = 1;

  try {
    switch (argc) {
      case 4:
        if (strcmp(argv[3], "d") != 0) {
          n_processors = convert(argv[3]);
          if (n_processors <= 0) throw cmd_error();
        }
      case 3:
        if (strcmp(argv[2], "d") != 0) {
          seed = convert(argv[2]);
          if (seed <= 0) throw cmd_error();
        }
      case 2:
        if (strcmp(argv[1], "d") != 0) {
          config_file = argv[1];
        }
      case 1:
        break;
      default:
        throw cmd_error();
    }
  } catch (...) {
    cerr << "Usage: " << argv[0]
         << " [ config-file (default soda.config) [ seed (> 0) | 'd' (default "
            "random) [ processors (> 0) | 'd' (default 1) ] ] ]"
         << endl;
    exit(EXIT_FAILURE);
  }

  // Set the number of processes
  uProcessor p[n_processors - 1] __attribute__((unused));

  // Set seed if passed in
  if (seed != -1) set_seed(seed);

  // Load in config file
  ConfigParms config;
  processConfigFile(config_file.c_str(), config);

  Printer printer(config.numStudents, config.numVendingMachines,
                  config.numCouriers);

  Bank bank(config.numStudents);

  Parent parent(printer, bank, config.numStudents, config.parentalDelay);

  WATCardOffice watcard_office(printer, bank, config.numCouriers);

  Groupoff groupoff(printer, config.numStudents, config.sodaCost,
                    config.groupoffDelay);

  NameServer name_server(printer, config.numVendingMachines,
                         config.numStudents);

  VendingMachine* vending_machines[config.numVendingMachines];
  for (unsigned int i = 0; i < config.numVendingMachines; i++) {
    vending_machines[i] =
        new VendingMachine(printer, name_server, i, config.sodaCost);
  }

  // Allocate on heap so we can explicitly delete later
  BottlingPlant* bottling_plant =
      new BottlingPlant(printer, name_server, config.numVendingMachines,
                        config.maxShippedPerFlavour, config.maxStockPerFlavour,
                        config.timeBetweenShipments);

  Student* students[config.numStudents];
  for (unsigned int i = 0; i < config.numStudents; i++) {
    students[i] = new Student(printer, name_server, watcard_office, groupoff, i,
                              config.maxPurchases);
  }

  // Delete all students
  for (unsigned int i = 0; i < config.numStudents; i++) {
    delete students[i];
  }

  // Delete bottling plant before vending machines
  delete bottling_plant;

  // Delete all vending machines
  for (unsigned int i = 0; i < config.numVendingMachines; i++) {
    delete vending_machines[i];
  }

  return 0;
}