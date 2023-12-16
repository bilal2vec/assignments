#ifndef PRINTER_H_
#define PRINTER_H_

_Monitor Printer {
  unsigned int num_students;
  unsigned int num_vending_machines;
  unsigned int num_couriers;

  void flush_buffer();

 public:
  enum Kind {
    Parent,
    Groupoff,
    WATCardOffice,
    NameServer,
    Truck,
    BottlingPlant,
    Student,
    Vending,
    Courier
  };
  Printer(unsigned int numStudents, unsigned int numVendingMachines,
          unsigned int numCouriers);
  ~Printer();
  void print(Kind kind, char state);
  void print(Kind kind, char state, unsigned int value1);
  void print(Kind kind, char state, unsigned int value1, unsigned int value2);
  void print(Kind kind, unsigned int lid, char state);
  void print(Kind kind, unsigned int lid, char state, unsigned int value1);
  void print(Kind kind, unsigned int lid, char state, unsigned int value1,
             unsigned int value2);

 private:
  struct DATA {
    bool is_cleared;
    Kind kind;
    char state;
    unsigned int value_1;
    unsigned int value_2;
  };
  DATA *data;
};

#endif  // PRINTER_H_