#include <iostream>
#include <string>
#include <fstream>
#include <math.h>
#include <algorithm>

#include "string.h"

#include <uPRNG.h>
#include <uActor.h>

#include "q2quicksort.h"

using namespace std;

int main(int argc, char* argv[]) {
	struct cmd_error {};
	
	// Check input validation for timing mode
	if (argc > 1 && strcmp(argv[1], "-t") == 0) {
		int size;
		int depth = 0;

		try {
			switch (argc) {
				case 4:
					depth = convert(argv[3]);
					if (depth < 0) throw cmd_error();
				case 3:
					size = convert(argv[2]);
					if (size < 0) throw cmd_error();
					break;
				default:
					throw cmd_error();
			}
		} catch(...) {
			cerr << "Usage: " << argv[0] << " ( [ unsorted-file | 'd' [ sorted-file | 'd' [ depth (>= 0) ] ] ] | -t size (>= 0) [ depth (>= 0) ]" << endl;
			exit(EXIT_FAILURE);
		}

		// Ceate and shuffle values
		STYPE* values = new STYPE[size];

		for (int i = 0; i < size; i++) {
			values[size - i - 1] = i;
		}

		unsigned int times = sqrt( size );
		for ( unsigned int counter = 0; counter < times; counter += 1 ) {
			swap( values[0], values[prng( size ) ] );
		}
		
		uProcessor p[ (1 << depth) - 1 ] __attribute__(( unused )); // 2^depth-1 kernel threads

		uTime start = uClock::currTime();

		// Sort using quicksort. Sorting with Actor requires a bit of extra setup
		#ifdef ACTOR
		uActor::start();
		*new QuickSort() | *new QuickSortMsg(values, 0, size - 1, depth) | uActor::stopMsg;
		uActor::stop();
		#else
		quicksort(values, 0, size - 1, depth);
		#endif

		cout << "Sort time " << uClock::currTime() - start << " sec." << endl;

		delete[] values;

	// Sorting mode
	} else{
		char* unsorted_file = nullptr;
		char* sorted_file = nullptr;
		int depth = 0;

		try {
			switch (argc) {
				case 4:
					depth = convert(argv[3]);
					if (depth < 0) throw cmd_error();
				case 3:
					sorted_file = argv[2];
				case 2:
					unsorted_file = argv[1];
				case 1:
					break;
				default:
					throw cmd_error();
			}
		} catch(...) {
			cerr << "Usage: " << argv[0] << " ( [ unsorted-file | 'd' [ sorted-file | 'd' [ depth (>= 0) ] ] ] | -t size (>= 0) [ depth (>= 0) ]" << endl;
			exit(EXIT_FAILURE);
		}


		istream *infile = &cin;
		ostream *outfile = &cout;

		if (unsorted_file != nullptr && strcmp(unsorted_file, "d") != 0) {
			try {
				infile = new ifstream(unsorted_file);
			} catch (uFile::Failure&) {
				cerr << "Error! Could not open unsorted input file \"" << unsorted_file << "\"" << endl;
				exit(EXIT_FAILURE);
			}
		}

		if (sorted_file != nullptr && strcmp(sorted_file, "d") != 0) {
			try {
				outfile = new ofstream(sorted_file);
			} catch (uFile::Failure&) {
				cerr << "Error! Could not open sorted output file \"" << sorted_file << "\"" << endl;
				exit(EXIT_FAILURE);
			}
		}

		int n_values = 0;
		STYPE value;

		while (*infile >> n_values) {
			STYPE* values = new STYPE[n_values];

			for (int i = 0; i < n_values; i++) {
				if (i > 0) {
					*outfile << " ";
				}
				*infile >> value;
				*outfile << value;

				values[i] = value;
			}
			*outfile << endl;

			// Sort using quicksort. Sorting with Actor requires a bit of extra setup
			#ifdef ACTOR
			uActor::start();
			*new QuickSort() | *new QuickSortMsg(values, 0, n_values - 1, depth) | uActor::stopMsg;
			uActor::stop();
			#else
			quicksort(values, 0, n_values - 1, depth);
			#endif

			for (int i = 0; i < n_values; i++) {
				if (i > 0) {
					*outfile << " ";
				}
				*outfile << values[i];
			}
			*outfile << endl;
	
			*outfile << endl;

			delete[] values;
		}

		if (infile != &cin) {
			delete infile;
		}

		if (outfile != &cout) {
			delete outfile;
		}

	}

	return 0;
}