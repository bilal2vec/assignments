#include "truck.h"

#include <algorithm>

#include "bottling_plant.h"
#include "name_server.h"
#include "printer.h"
#include "vending_machine.h"

using namespace std;

void Truck::main() {
  printer.print(Printer::Kind::Truck, 'S');

  // This is our array storing the number of bottles we're transporing
  unsigned int capacity[4];
  VendingMachine** vending_machines = name_server.getMachineList();
  unsigned int current_machine_idx = 0;

  while (true) {
    // Exit if destructor called
    _Accept(~Truck) { break; }
    _Else {
      // Go to bims
      yield(prng(1, 10));

      // Get the shipment from the bottling plant, handle shutdown gracefullys
      try {
        bottling_plant.getShipment(capacity);
      } catch (BottlingPlant::Shutdown& e) {
        break;
      }

      // Calculate the total number of bottles we're transporting
      unsigned int total_bottles = 0;
      for (unsigned int i = 0; i < 4; i++) {
        total_bottles += capacity[i];
      }

      printer.print(Printer::Kind::Truck, 'P', total_bottles);

      // Restock the vending machines in a round robin fashion
      for (unsigned int i = 0; i < num_vending_machines; i++) {
        // Stop if we're out of bottles
        if (total_bottles == 0) break;

        // Call inventory to offically start refilling
        VendingMachine* vending_machine = vending_machines[current_machine_idx];
        unsigned int* inventory = vending_machine->inventory();

        printer.print(Printer::Kind::Truck, 'd', vending_machine->getId(),
                      total_bottles);

        // Keep track of how many empty bottle slots we are unable to fill
        unsigned int empty_slots_remaining = 0;

        // Fill each flavour slot with as many bottles as possible
        for (unsigned int j = 0; j < 4; j++) {
          unsigned int num_bottles_to_fill =
              min(capacity[j], max_stock_per_flavour - inventory[j]);
          empty_slots_remaining += max_stock_per_flavour - inventory[j];

          inventory[j] += num_bottles_to_fill;
          capacity[j] -= num_bottles_to_fill;
          total_bottles -= num_bottles_to_fill;

          vending_machine->restocked();
        }

        current_machine_idx = (current_machine_idx + 1) % num_vending_machines;

        // Print the number of empty slots remaining *only if applicable*
        if (empty_slots_remaining > 0) {
          printer.print(Printer::Kind::Truck, 'U', vending_machine->getId(),
                        empty_slots_remaining);
        }

        printer.print(Printer::Kind::Truck, 'D', vending_machine->getId(),
                      total_bottles);

        // Flat tire
        if (prng(99) == 0) {
          printer.print(Printer::Kind::Truck, 'W');
          yield(10);
        }
      }
    }
  }

  printer.print(Printer::Kind::Truck, 'F');
}

Truck::Truck(Printer& prt, NameServer& nameServer, BottlingPlant& plant,
             unsigned int numVendingMachines, unsigned int maxStockPerFlavour)
    : printer(prt),
      name_server(nameServer),
      bottling_plant(plant),
      num_vending_machines(numVendingMachines),
      max_stock_per_flavour(maxStockPerFlavour) {}