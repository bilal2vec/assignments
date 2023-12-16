#ifndef VENDING_MACHINE_H_
#define VENDING_MACHINE_H_

#include "bottling_plant.h"

_Monitor Printer;
_Task NameServer;
class WATCard;

enum ExceptionType { None, Stock, Funds, Free };

_Task VendingMachine {
  Printer & printer;
  NameServer & name_server;
  unsigned int id;
  unsigned int soda_cost;
  unsigned int n_inventory[4];
  bool allow_buy;
  ExceptionType exception_type;

  unsigned int flavor;
  WATCard *watcard;

  uCondition bench;

  void main();

 public:
  _Event Free{};   // free, advertisement
  _Event Funds{};  // insufficient funds
  _Event Stock{};  // flavour out of stock
  VendingMachine(Printer & prt, NameServer & nameServer, unsigned int id,
                 unsigned int sodaCost);
  void buy(BottlingPlant::Flavours flavour, WATCard & card);
  unsigned int *inventory() __attribute__((warn_unused_result));
  void restocked();
  _Nomutex unsigned int cost() const;
  _Nomutex unsigned int getId() const;
};

#endif  // VENDING_MACHINE_H_