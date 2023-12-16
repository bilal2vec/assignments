#include <iostream>
#include <fstream>
#include <iomanip>
#include <cmath>

using namespace std;

#include "q3floatconstant.h"

int main(int argc, char * argv[]) {
    // Setup input stream to read from stdin by default
	istream * infile = &cin;

	struct cmd_error {};
    try{
        if (argc > 1) {
            // Setup input stream to read from file if provided
            try {
                infile = new ifstream(argv[1]);

            } catch (uFile::Failure&) {
                cerr << "Error! Could not open input file \"" << argv[1] << "\"" << endl;
                throw cmd_error();
            }
        }
    } catch(...) {
        cerr << "Usage: " << argv[0] << " [infile-file]" << endl;
		exit(EXIT_FAILURE);
    }

    // Skip whitespace
    *infile >> noskipws;

    string line;

    // Read in file line by line
    while (getline(*infile, line)) {

        // Skip blank lines and print warnings
        if (line == "") {
            cout << "\"" << line << "\" : Warning! Blank line." << endl;
            continue;
        }

        // Keep track of last index of line that was parsed successfully
        int last_idx = 0;

        cout << "\"" << line << "\" : \"";

        try {
            // Enable non-local exceptions (i.e. Match and Error) to be thrown in coroutine and caught here
            _Enable  {
                FloatConstant parser;
                for (char c : line) {
                    last_idx += 1;
                    parser.next(c);
                }

                parser.next(FloatConstant::EOT);
            }

        }
        // Catch Match and Error exceptions thrown by coroutine
        catch(FloatConstant::Match &m) {
            cout << line.substr(0, last_idx) << "\" yes, value " << setprecision(16) << m.value;
        } catch(FloatConstant::Error &) {
            cout << line.substr(0, last_idx) << "\" no";
        } 

        // Print extraneous characters if any
        string extraneous = line.substr(last_idx);
        if (extraneous.length() > 0) {
            cout << " -- extraneous characters \"" << extraneous << "\"";
        }

        cout << endl;
    }


    // Close input file if it was opened to prevent memory leaks
	if ( infile != &cin ) {
        delete infile;
    }
}