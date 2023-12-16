#include <uPRNG.h>

#include <iostream>

#include "q2_printer.h"
#include "q2_voter.h"
#include "q2tallyVotes.h"

using namespace std;

int main(int argc, char* argv[]) {
  struct cmd_error {};

  int n_voters = 6;
  int group_size = 3;
  int n_votes = 1;
  int seed = getpid();
  int n_processors = 1;

  try {
    switch (argc) {
      case 6:
        if (strcmp(argv[5], "d") != 0) {
          n_processors = convert(argv[5]);
          if (n_processors <= 0) throw cmd_error();
        }
      case 5:
        if (strcmp(argv[4], "d") != 0) {
          seed = convert(argv[4]);
          if (seed <= 0) throw cmd_error();
        }
      case 4:
        if (strcmp(argv[3], "d") != 0) {
          n_votes = convert(argv[3]);
          if (n_votes <= 0) throw cmd_error();
        }
      case 3:
        if (strcmp(argv[2], "d") != 0) {
          group_size = convert(argv[2]);
          if (group_size <= 0) throw cmd_error();
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
    cerr << "Usage: " << argv[0]
         << " [ voters (> 0) | 'd' (default 6) [ group (> 0) | 'd' (default 3) "
            "[ votes (> 0) | 'd' (default 1) [ seed (> 0) | 'd' (default "
            "random) [ processors (> 0) | 'd' (default 1) ] ] ] ] ]"
         << endl;
    exit(EXIT_FAILURE);
  }

  // Set the number of processes
  uProcessor p[n_processors - 1] __attribute__((unused));

  // Set seed for global thread-safe prng
  // set_seed(seed);

  // Create printer and vote tallier
  Printer printer(n_voters);
  TallyVotes tally_votes(n_voters, group_size, printer);

  // Create voters, use block scope to force them to join
  {
    uNoCtor<Voter> voters[n_voters];

    for (int i = 0; i < n_voters; i++) {
      voters[i].ctor(i, n_votes, tally_votes, printer);
    }
  }

  return 0;
}