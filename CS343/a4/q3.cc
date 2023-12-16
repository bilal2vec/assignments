#include <uPRNG.h>

#include <iostream>

#include "q3printer.h"
#include "q3tallyVotes.h"
#include "q3voter.h"

using namespace std;

int main(int argc, char* argv[]) {
  struct cmd_error {};

  // Default values
  int n_voters = 6;
  int group = 3;
  int votes = 1;
  int seed = getpid();
  int processors = 1;

  // Parse arguments
  try {
    switch (argc) {
      case 6:
        if (strcmp(argv[5], "d") != 0) {
          processors = convert(argv[5]);
          if (processors <= 0) throw cmd_error();
        }
      case 5:
        if (strcmp(argv[4], "d") != 0) {
          seed = convert(argv[4]);
          if (seed <= 0) throw cmd_error();
        }
      case 4:
        if (strcmp(argv[3], "d") != 0) {
          votes = convert(argv[3]);
          if (votes <= 0) throw cmd_error();
        }
      case 3:
        if (strcmp(argv[2], "d") != 0) {
          group = convert(argv[2]);
          if (group <= 0) throw cmd_error();
        }
      case 2:
        if (strcmp(argv[1], "d") != 0) {
          n_voters = convert(argv[1]);
          if (n_voters <= 0) throw cmd_error();
        }
      case 1:
        break;
      default:
        throw cmd_error();
    }
  } catch (...) {
    cerr << "Usage: " << argv[0] << " [ voters (> 0) | 'd' (default 6) [ group (> 0) | 'd' (default 3) [ votes (> 0) | 'd' (default 1) [ seed (> 0) | 'd' (default random) [ processors (> 0) | 'd' (default 1) ] ] ] ] ]" << endl;
    exit(EXIT_FAILURE);
  }

  // Set the number of processes
  uProcessor p[processors - 1] __attribute__(( unused ));

  // Set seed for global thread-safe prng
  set_seed(seed);

  // Create printer and vote tallyer
  Printer printer(n_voters);
  TallyVotes tally_votes(n_voters, group, printer);

  // Create voters, use block scope to force join
  {
    uNoCtor<Voter> voters[n_voters];

    for (int i = 0; i < n_voters; i++) {
      voters[i].ctor(i, votes, tally_votes, printer);
    }
  }

  return 0;  

}