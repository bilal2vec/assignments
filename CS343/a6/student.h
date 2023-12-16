#ifndef STUDENT_H_
#define STUDENT_H_

_Monitor Printer;
_Task NameServer;
_Task WATCardOffice;
_Task Groupoff;

_Task Student {
  Printer & printer;
  NameServer & name_server;
  WATCardOffice & watcard_office;
  Groupoff & groupoff;
  unsigned int id;
  unsigned int max_purchases;

  void main();

 public:
  Student(Printer & prt, NameServer & nameServer, WATCardOffice & cardOffice,
          Groupoff & groupoff, unsigned int id, unsigned int maxPurchases);
};

#endif  // STUDENT_H_