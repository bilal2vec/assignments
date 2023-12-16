#include "q2_printer.h"

#include <iostream>

using namespace std;

void Printer::flush_buffer() {
  for (unsigned int i = 0; i < voters; i++) {
#ifndef NOOUTPUT
    // Align each column to multiple of \t
    if (i > 0) {
      cout << "\t";
    }

    // If the state for the voter is empty, skip
    if (voter_structs[i].is_cleared) {
      continue;
    }

    // Print out the voter's state char
    char voter_status = voter_structs[i].state;
    cout << voter_status;

    // Pritn out voter state-specific data
    switch (voter_structs[i].state) {
      case Voter::States::Start:
        break;
      case Voter::States::Vote: {
        TallyVotes::Ballot ballot = voter_structs[i].data.ballot;
        cout << " " << ballot.picture << "," << ballot.statue << ","
             << ballot.giftshop;
        break;
      }
      case Voter::States::Block: {
        BLOCK_DATA block_data = voter_structs[i].data.block_data;
        cout << " " << block_data.n;
        break;
      }
      case Voter::States::Unblock: {
        BLOCK_DATA block_data = voter_structs[i].data.block_data;
        cout << " " << block_data.n;
        break;
      }
      case Voter::States::Barging: {
        BARGING_DATA barging_data = voter_structs[i].data.barging_data;
        cout << " " << barging_data.n << "," << barging_data.gn;
        break;
      }
      case Voter::States::Done:
        break;
      case Voter::States::Complete: {
        TallyVotes::Tour tour = voter_structs[i].data.tour;
        cout << " " << tour.tourkind;
        break;
      }
      case Voter::States::Going: {
        TallyVotes::Tour tour = voter_structs[i].data.tour;
        cout << " " << tour.tourkind << " " << tour.groupno;
        break;
      }
      case Voter::States::Failed:
        break;
      case Voter::States::Terminated:
        break;
      default:
        // This should never happen
        throw 1;
    }
#endif

    voter_structs[i].is_cleared = true;
  }

#ifndef NOOUTPUT
  cout << endl;
#endif
}

Printer::Printer(unsigned int voters) : voters(voters) {
  voter_structs = new VoterStruct[voters];

  for (unsigned int i = 0; i < voters; i++) {
    voter_structs[i].is_cleared = true;
  }

#ifndef NOOUTPUT
  // Print out first line of header
  for (unsigned int i = 0; i < voters; i++) {
    cout << "V" << i << "\t";
  }
  cout << endl;

  // Print out second line of header, seven asterisks
  for (unsigned int i = 0; i < voters; i++) {
    cout << "*******"
         << "\t";
  }
  cout << endl;
#endif
}

Printer::~Printer() {
  // Flush any remaining data in the buffer
  flush_buffer();

  delete[] voter_structs;

#ifndef NOOUTPUT
  // Print out footer, 17 asterisks
  cout << "*****************" << endl;
  cout << "All tours ended" << endl;
#endif
}

void Printer::print(unsigned int id, Voter::States state) {
  if (!voter_structs[id].is_cleared) {
    flush_buffer();
  }

  voter_structs[id].is_cleared = false;
  voter_structs[id].state = state;
}

void Printer::print(unsigned int id, Voter::States state,
                    TallyVotes::Tour tour) {
  print(id, state);

  voter_structs[id].data.tour = tour;
}

void Printer::print(unsigned int id, Voter::States state,
                    TallyVotes::Ballot vote) {
  print(id, state);

  voter_structs[id].data.ballot = vote;
}

void Printer::print(unsigned int id, Voter::States state,
                    unsigned int numBlocked) {
  print(id, state);

  BLOCK_DATA data;
  data.n = numBlocked;

  voter_structs[id].data.block_data = data;
}

void Printer::print(unsigned int id, Voter::States state,
                    unsigned int numBlocked, unsigned int group) {
  print(id, state);

  BARGING_DATA data;
  data.n = numBlocked;
  data.gn = group;

  voter_structs[id].data.barging_data = data;
}