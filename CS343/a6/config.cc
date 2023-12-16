#include <fstream>
#include <iostream>
#include <limits>  // numeric_limits
using namespace std;
#include "config.h"

static bool comments(ifstream& in, string& name) {
  for (;;) {
    in >> name;
    if (in.fail()) return true;
    if (name.substr(0, 1) != "#") break;
    in.ignore(numeric_limits<int>::max(), '\n');  // ignore remainder of line
  }                                               // for
  return false;
}  // comments

// Process the configuration file to set the simulation parameters.
void processConfigFile(const char* configFile, ConfigParms& cparms) {
  enum { Parmnum = 10 };
  struct {
    const char* name;     // configuration name
    bool used;            // already supplied ?
    unsigned int& value;  // location to put configuration value
  } parms[Parmnum] = {
      {"SodaCost", false, cparms.sodaCost},
      {"NumStudents", false, cparms.numStudents},
      {"MaxPurchases", false, cparms.maxPurchases},
      {"NumVendingMachines", false, cparms.numVendingMachines},
      {"MaxStockPerFlavour", false, cparms.maxStockPerFlavour},
      {"MaxShippedPerFlavour", false, cparms.maxShippedPerFlavour},
      {"TimeBetweenShipments", false, cparms.timeBetweenShipments},
      {"GroupoffDelay", false, cparms.groupoffDelay},
      {"ParentalDelay", false, cparms.parentalDelay},
      {"NumCouriers", false, cparms.numCouriers},
  };
  string name;
  int value;
  unsigned int cnt, posn, numOfParm = 0;

  try {
    ifstream in(configFile);  // open the configuration file for input

    for (cnt = 0; cnt < Parmnum;
         cnt += 1) {                  // parameter names can appear in any order
      if (comments(in, name)) break;  // eof ?
      for (posn = 0; posn < Parmnum && name != parms[posn].name; posn += 1)
        ;                           // linear search
      if (posn == Parmnum) break;   // configuration not found ?
      if (parms[posn].used) break;  // duplicate configuration ?
      in >> value;
      if (value <= 0) {
        cerr << "Error: file \"" << configFile << "\" parameter " << name
             << " value " << value << " must be positive." << endl;
        exit(EXIT_FAILURE);
      }  // if
      if (in.fail()) break;
      in.ignore(numeric_limits<int>::max(), '\n');  // ignore remainder of line
      numOfParm += 1;
      parms[posn].used = true;
      parms[posn].value = value;
    }  // for

    if (numOfParm != Parmnum) {
      cerr << "Error: file \"" << configFile << "\" is corrupt." << endl;
      exit(EXIT_FAILURE);
    }                           // if
    if (!comments(in, name)) {  // ! eof ?
      cerr << "Error: file \"" << configFile << "\" has extraneous data."
           << endl;
      exit(EXIT_FAILURE);
    }  // if
  } catch (uFile::Failure&) {
    cerr << "Error: could not open input file \"" << configFile << "\"" << endl;
    exit(EXIT_FAILURE);
  }  // try
}  // processConfigFile