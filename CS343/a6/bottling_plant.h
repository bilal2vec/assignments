#ifndef BOTTLING_PLANT_H_
#define BOTTLING_PLANT_H_

_Monitor Printer;
_Task NameServer;

_Task BottlingPlant {
  Printer & printer;
  NameServer & name_server;
  unsigned int num_vending_machines;
  unsigned int max_shipped_per_flavour;
  unsigned int max_stock_per_flavour;
  unsigned int time_between_shipments;
  bool should_throw;
  unsigned int n_bottles_produced;

  void main();

 public:
  enum Flavours {
    CHERRY = 0,
    CREAM_SODA = 1,
    ROOT_BEER = 2,
    LIME = 3
  };                  // flavours of soda
  _Event Shutdown{};  // shutdown plant
  BottlingPlant(
      Printer & prt, NameServer & nameServer, unsigned int numVendingMachines,
      unsigned int maxShippedPerFlavour, unsigned int maxStockPerFlavour,
      unsigned int timeBetweenShipments);
  void getShipment(unsigned int cargo[]);
};

#endif  // BOTTLING_PLANT_H_