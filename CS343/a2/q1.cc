#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>

#include "q1binsertsort.h"

using namespace std;

typedef struct Point {
	int x, y;

	Point(): x(0), y(0) {}
	Point(int x, int y) : x(x), y(y) {}

	bool operator==(const Point& other) const {
		return x == other.x && y == other.y;
	}

	bool operator<(const Point& other) const {
		double dist = sqrt(x*x + y*y);
		double other_dist = sqrt(other.x*other.x + other.y*other.y);
		return dist < other_dist;
	}

	friend istream& operator>>(istream& is, Point& p) {
		return is >> p.x >> p.y;
	}

	friend ostream& operator<<(ostream& os, const Point& p) {
		return os << "(" << p.x << "," << p.y << ")";
	}
} Point;

int main(int argc, char * argv[]) {

	// If no input file is provided return error
	if (argc < 2) {
		cerr << "Usage: " << argv[0] << " unsorted-file [ sorted-file ]" << endl;
		exit(EXIT_FAILURE);
	}

    ifstream *infile;

	try {
		infile = new ifstream(argv[1]);

	} catch (uFile::Failure&) {
		cerr << "Error! Could not open input file \"" << argv[1] << "\"" << endl;
		exit(EXIT_FAILURE);
	}

	// Use cout if no output file is provided
	ostream * outfile = &cout;
	if (argc > 2) {
		outfile = new ofstream(argv[2]);
	}

	int n_values = 0;
	TYPE value;

	// For each list in the file sort it using Binsertsort
	while (*infile >> n_values) {
		Binsertsort<TYPE> binsertsort;

		for (int i = 0; i < n_values; i++) {
			if (i > 0) {
				*outfile << " ";
			}

			*infile >> value;
			*outfile << value;

			binsertsort.sort(value);
		}
		*outfile << endl;

		// Retrieve sorted list by repeatedly calling retrieve() until Sentinel is thrown
		try {
			_Enable {
				_Resume Binsertsort<TYPE>::Sentinel() _At binsertsort;
				
				value = binsertsort.retrieve();
				*outfile << value;
			
				while (1) {
					value = binsertsort.retrieve();
					*outfile << " " << value;
				}
			}
		} catch(Binsertsort<TYPE>::Sentinel &e) {
			*outfile << endl;
		}

		*outfile << endl;
	}

	// Delete file ptrs
	delete infile;
	if (outfile != &cout) {
		delete outfile;
	}
}