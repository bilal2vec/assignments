#include "bottling_plant.h"

#include "name_server.h"
#include "printer.h"
#include "truck.h"

void BottlingPlant::main() {
  // Create truck
  Truck truck(printer, name_server, *this, num_vending_machines,
              max_stock_per_flavour);

  printer.print(Printer::Kind::BottlingPlant, 'S');

  while (true) {
    // According to sample outputs, therer is a big gap between prints of G and
    // P, probably bc you're supposed to yield in between
    // Print the *total* number of bottles produced, not the number of bottles
    // per flavor
    n_bottles_produced = prng(max_shipped_per_flavour);
    printer.print(Printer::Kind::BottlingPlant, 'G', n_bottles_produced * 4);

    // Yield to simulate a production run
    printer.print(Printer::Kind::BottlingPlant, 'G');
    yield(time_between_shipments);

    // Exit if destructor called
    _Accept(~BottlingPlant) { break; }
    // Allow truck to pick up shipment
    or _Accept(getShipment) {}
  }

  // Accept one last getShipment call to throw Shutdown exception to
  // gracefully exit
  should_throw = true;
  try {
    _Accept(getShipment) {}
  } catch (uMutexFailure::RendezvousFailure& e) {
  }

  printer.print(Printer::Kind::BottlingPlant, 'F');
}

BottlingPlant::BottlingPlant(Printer& prt, NameServer& nameServer,
                             unsigned int numVendingMachines,
                             unsigned int maxShippedPerFlavour,
                             unsigned int maxStockPerFlavour,
                             unsigned int timeBetweenShipments)
    : printer(prt),
      name_server(nameServer),
      num_vending_machines(numVendingMachines),
      max_shipped_per_flavour(maxShippedPerFlavour),
      max_stock_per_flavour(maxStockPerFlavour),
      time_between_shipments(timeBetweenShipments),
      should_throw(false),
      n_bottles_produced(0) {}

void BottlingPlant::getShipment(unsigned int cargo[]) {
  // On the last shipment, throw Shutdown exception to gracefully exit
  if (should_throw) {
    _Throw Shutdown();
  }

  for (unsigned int i = 0; i < 4; i++) {
    cargo[i] = n_bottles_produced;
  }

  // Print that the truck is picking up the shipment
  printer.print(Printer::Kind::BottlingPlant, 'P');
}