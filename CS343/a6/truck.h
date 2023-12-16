#ifndef TRUCK_H_
#define TRUCK_H_

_Monitor Printer;
_Task NameServer;
_Task BottlingPlant;

_Task Truck {
  Printer & printer;
  NameServer & name_server;
  BottlingPlant & bottling_plant;
  unsigned int num_vending_machines;
  unsigned int max_stock_per_flavour;

  void main();

 public:
  Truck(Printer & prt, NameServer & nameServer, BottlingPlant & plant,
        unsigned int numVendingMachines, unsigned int maxStockPerFlavour);
};

#endif  // TRUCK_H_