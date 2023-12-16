#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <cctype> // toupper()
using namespace std;

class Justifier
{
public:
    virtual ~Justifier();
    virtual void justify();

    enum Style
    {
        RaggedRight,
        RaggedLeft,
        Centred,
        Smooth,
        Verbatim
    };
    static const vector<string> StyleName;
    static const int DefaultMaxLineLength;
    static const int MinLineLength;
    static const Style DefaultStyle;

protected:
    Justifier(int MaxLineLength, ifstream &instream, ofstream &outstream);
    string getNextInputChunk();
    bool moreInputRemains() const;

    const int MaxLineLength;
    ifstream &instream;

private:
    virtual string getNextLine() = 0;
    virtual string justifyLine(const string &line) = 0;
    ofstream &outstream;

    // A chunk is the unit we read in from the raw source file:
    //	-- a token for TokenBasedJustifier, and
    //	-- a line for VerbatimJustifier.
    // A single chunk can be longer than MaxLineLength, so we have to
    // be careful how we process it, keeping the remaining bits around
    // for the next call to getNextInputChunk
    virtual string readNextInputChunk() = 0;
    string curChunk;
};

const vector<string> Justifier::StyleName = {"Ragged right", "Ragged left",
                                             "Centred", "Smooth", "Verbatim"};
const int Justifier::DefaultMaxLineLength = 80;
const int Justifier::MinLineLength = 5;
const Justifier::Style Justifier::DefaultStyle = Justifier::RaggedRight;

// just initialize the member variables
Justifier::Justifier(int MaxLineLength, ifstream &instream, ofstream &outstream) : MaxLineLength(MaxLineLength), instream(instream), outstream(outstream), curChunk("") {}

Justifier::~Justifier() {}

// Implement this!
// Returns true iff we have read all of the chunks from the input file
// *and* we have processed all of the (possibly very long) tokens
bool Justifier::moreInputRemains() const
{
    return (instream.peek() != EOF) || (curChunk.size() != 0);
}

void Justifier::justify()
{
    string nextLine = getNextLine();
    while (moreInputRemains() || nextLine != "")
    {
        outstream << justifyLine(nextLine) << endl;
        nextLine = getNextLine();
    }
}

// Implement this!
// Returns the next logical chunk of input (trimmed to length if needed).
// This might simply process the rest of a previously-read-in chunk or it
// might cause a call to readNextInputChunk.
string Justifier::getNextInputChunk()
{
    string next_chunk;

    // check if current chunk is empty
    // if it is, just read the next chunk from the input
    // otherwise take the first `MaxLineLength` characters from the current chunk as the next chunk
    if (this->curChunk.size() > 0)
    {
        next_chunk = this->curChunk.substr(0, (this->MaxLineLength - 1));
        this->curChunk = this->curChunk.substr((this->MaxLineLength - 1));
        return next_chunk;
    }
    else
    {
        return this->readNextInputChunk();
    }
}

// An intermediate abstract base class that all of the concrete Justifiers
// inherit from (except VerbatimJustifier, which is line based).  We can
// fully define readNextInputChunk and getNextLine here, and leave the
// remaining token-based classes to implement only justifyLine
class TokenBasedJustifier : public Justifier
{
public:
    TokenBasedJustifier(int MaxLineLength, ifstream &instream,
                        ofstream &outstream);
    virtual ~TokenBasedJustifier();

private:
    virtual string getNextLine() override;
    virtual string readNextInputChunk() override;
    string toupper(const string &s); // like ::toupper but for strings
    string curLine;                  // holds leftover bits of long lines
    bool boldfaceOn;
};

// use the parent's constructor to initialize this child class
TokenBasedJustifier::TokenBasedJustifier(int MaxLineLength, ifstream &instream, ofstream &outstream) : Justifier(MaxLineLength, instream, outstream), curLine(""), boldfaceOn(false) {
}

TokenBasedJustifier::~TokenBasedJustifier() {}

// convert each character in the string to upper case and return the new concatenated string
string TokenBasedJustifier::toupper(const string &s)
{
    string new_string = "";
    for (int i = 0; i < s.size(); i++)
    {
        char new_char = std::toupper(s[i]);
        new_string.push_back(new_char);
    }
    return new_string;
}

// take the next token from the input file stream and return it
string TokenBasedJustifier::readNextInputChunk()
{
    string token;
    this->instream >> token;

    if ((token == "<bf>")) {
        this->boldfaceOn = true;
        return this->readNextInputChunk();
    } else if ((token == "</bf>")) {
        this->boldfaceOn = false;
        return this->readNextInputChunk();
    } 

    return token;
}

string TokenBasedJustifier::getNextLine()
{
    vector<string> next_line{};
    int remaining_chars = this->MaxLineLength;

    // check if there are any tokens in the current line
    if (this->curLine.size() > 0)
    {
        // hyphenate and return the current line if necessary
        if (this->curLine.size() > remaining_chars)
        {
            string temp = this->curLine.substr(0, remaining_chars - 1);
            this->curLine = this->curLine.substr(remaining_chars - 1);

            return temp + "-";
        }

        if (this->curLine == "<hr>") {
            string token = "";
            for (int i = 0; i < this->MaxLineLength; i++)
            {
                token += "-";
            }
            this->curLine = "";
            return token;
        }

        // otherwise grab all the chars from the current line as the first token in the line
        next_line.push_back(this->curLine);
        remaining_chars -= this->curLine.size();
        this->curLine = "";
    }
    // otherwise if the current line is empty then grab the next input chunk, hyphenate if necessary and add that as the first token in the line
    else
    {
        string next_token = this->getNextInputChunk();
        
        if (this->boldfaceOn) {
            next_token = this->toupper(next_token);
        }

        if (next_token == "<br>") {
            return "";
        }

        if (next_token == "<hr>") {
            string token = "";
            for (int i = 0; i < this->MaxLineLength; i++)
            {
                token += "-";
            }
            return token;
        }

        if (next_token.size() > remaining_chars)
        {
            string temp = next_token.substr(0, remaining_chars - 1);
            this->curLine = next_token.substr(remaining_chars - 1);

            return temp + "-";
        }

        next_line.push_back(next_token);
        remaining_chars -= next_token.size();
    }

    // as long as we have space in the current line keep grabbing more tokens from the file and adding them to the current line
    // save any tokens that go beyond the max length of the line to `curLine`
    while (remaining_chars > 0)
    {
        string next_token = this->getNextInputChunk();

        if (this->boldfaceOn) {
            next_token = this->toupper(next_token);
        }

        if (next_token == "")
        {
            break;
        } else if (next_token == "<br>") {
            break;
        } else if (next_token == "<hr>") {
            this->curLine = "<hr>";
            break;
        }

        if ((next_token.size() + 1) <= remaining_chars)
        {
            next_line.push_back(next_token);
            remaining_chars -= 1; // for space between words
            remaining_chars -= next_token.size();
        }
        else
        {
            remaining_chars = 0;
            this->curLine = next_token;
        }
    }

    // add a single space between words and return the line
    string next_line_str = next_line[0];
    for (int i = 1; i < next_line.size(); i++)
    {
        next_line_str.append(" ");
        next_line_str.append(next_line[i]);
    }

    return next_line_str;
}

// use the parent's constructor to initialize this child class
class RaggedRightJustifier : public TokenBasedJustifier
{
public:
    RaggedRightJustifier(int MaxLineLength, ifstream &instream,
                         ofstream &outstream);
    virtual ~RaggedRightJustifier();

private:
    virtual string justifyLine(const string &line) override;
};

RaggedRightJustifier::RaggedRightJustifier(int MaxLineLength, ifstream &instream, ofstream &outstream) : TokenBasedJustifier(MaxLineLength, instream, outstream) {}

RaggedRightJustifier::~RaggedRightJustifier() {}

// the ragged right justifier is already justified as is :)
string RaggedRightJustifier::justifyLine(const string &line)
{
    return line;
}

class RaggedLeftJustifier : public TokenBasedJustifier
{
public:
    RaggedLeftJustifier(int MaxLineLength, ifstream &instream,
                        ofstream &outstream);
    virtual ~RaggedLeftJustifier();

private:
    virtual string justifyLine(const string &line) override;
};

// use the parent's constructor to initialize this child class
RaggedLeftJustifier::RaggedLeftJustifier(int MaxLineLength, ifstream &instream, ofstream &outstream) : TokenBasedJustifier(MaxLineLength, instream, outstream) {}

RaggedLeftJustifier::~RaggedLeftJustifier() {}

// add the required number of spaces to the beginning of each line
string RaggedLeftJustifier::justifyLine(const string &line)
{
    int diff = this->MaxLineLength - line.size();
    string spaces = "";
    for (int i = 0; i < diff; i++)
    {
        spaces.append(" ");
    }
    return spaces + line;
}

class CentredJustifier : public TokenBasedJustifier
{
public:
    CentredJustifier(int MaxLineLength, ifstream &instream,
                     ofstream &outstream);
    virtual ~CentredJustifier();

private:
    virtual string justifyLine(const string &line) override;
};

// use the parent's constructor to initialize this child class
CentredJustifier::CentredJustifier(int MaxLineLength, ifstream &instream, ofstream &outstream) : TokenBasedJustifier(MaxLineLength, instream, outstream) {}

CentredJustifier::~CentredJustifier() {}

// add the required number of spaces to the beginning and end of each line
string CentredJustifier::justifyLine(const string &line)
{
    int diff = this->MaxLineLength - line.size();

    int left_pad = 0;
    int right_pad = 0;

    if (diff % 2 == 0)
    {
        left_pad = diff / 2;
        right_pad = diff / 2;
    }
    else
    {
        left_pad = (diff - 1) / 2;
        right_pad = (diff - 1) / 2;
        left_pad += 1;
    }

    string left_spaces = "";
    string right_spaces = "";

    for (int i = 0; i < left_pad; i++)
    {
        left_spaces.append(" ");
    }

    for (int i = 0; i < right_pad; i++)
    {
        right_spaces.append(" ");
    }

    return left_spaces + line + right_spaces;
}

class SmoothJustifier : public TokenBasedJustifier
{
public:
    SmoothJustifier(int MaxLineLength, ifstream &instream,
                    ofstream &outstream);
    virtual ~SmoothJustifier();

private:
    virtual string justifyLine(const string &line) override;
};

// use the parent's constructor to initialize this child class
SmoothJustifier::SmoothJustifier(int MaxLineLength, ifstream &instream, ofstream &outstream) : TokenBasedJustifier(MaxLineLength, instream, outstream) {}

SmoothJustifier::~SmoothJustifier() {}

// Implement this!
string SmoothJustifier::justifyLine(const string &line)
{
    int diff = this->MaxLineLength - line.size();

    if (diff == 0)
    {
        return line;
    }

    vector<string> words{};
    string current_word = "";
    for (int i = 0; i < line.size(); i++)
    {
        if (line[i] == ' ')
        {
            words.push_back(current_word);
            current_word = "";
        }
        else
        {
            current_word += line[i];
        }
    }
    words.push_back(current_word);

    vector<int> spaces{};
    for (int i = 0; i < (words.size() - 1); i++)
    {
        spaces.push_back(1);
    }

    if (spaces.size() == 0)
    {
        string extra_spaces_at_end = "";
        for (int i = 0; i < diff; i++)
        {
            extra_spaces_at_end += " ";
        }

        return line + extra_spaces_at_end;
    }

    while (diff > 0)
    {
        for (int i = 0; i < spaces.size(); i++)
        {
            spaces[i] += 1;
            diff -= 1;

            if (diff == 0)
            {
                break;
            }
        }
    }

    string new_line = words[0];
    for (int i = 1; i < words.size(); i++)
    {
        string temp = "";
        for (int j = 0; j < spaces[i - 1]; j++)
        {
            temp += " ";
        }

        new_line = new_line + temp + words[i];
    }

    return new_line;
}

// The classes LineBasedJustifer and VerbatimJustifier do not have to be
// implemented for Part 1; de-comment and implement them for Part II.

// LineBasedJustifer is an intermediate abstract base class that is an
// alternative to TokenBasedJustifier; its only concrete child class is
// VerbatimJustifier.  We fully define readNextInputChunk and getNextLine
// here, and leave the VerbatimJustifier to implement only justifyLine.

class LineBasedJustifer : public Justifier
{
public:
    LineBasedJustifer(int MaxLineLength, ifstream &instream,
                      ofstream &outstream);
    virtual ~LineBasedJustifer();

private:
    virtual string getNextLine() override;
    virtual string readNextInputChunk() override;
    string curLine; // holds the leftover bits of long lines
};

// call parent's constructor
LineBasedJustifer::LineBasedJustifer(int MaxLineLength, ifstream &instream, ofstream &outstream) : Justifier(MaxLineLength, instream, outstream) {}

LineBasedJustifer::~LineBasedJustifer() {}

// take the next line from the input file stream and return it
string LineBasedJustifer::readNextInputChunk()
{
    string line;
    getline(this->instream, line);
    return line;
}

string LineBasedJustifer::getNextLine()
{
    int remaining_chars = this->MaxLineLength;

    // check if the current line is empty
    if (this->curLine.size() > 0)
    {
        // hyphenate and return the current line if necessary
        if (this->curLine.size() > remaining_chars)
        {
            string temp = this->curLine.substr(0, remaining_chars - 1);
            this->curLine = this->curLine.substr(remaining_chars - 1);

            return temp + "-";
        }

        // return the current line and clear the buffer
        string temp = this->curLine;
        this->curLine = "";
        return temp;
    }
    // otherwise if the current line is empty then grab the next input chunk, hyphenate if necessary and return it
    else
    {
        string next_token = this->getNextInputChunk();

        if (next_token.size() > remaining_chars)
        {
            string temp = next_token.substr(0, remaining_chars - 1);
            this->curLine = next_token.substr(remaining_chars - 1);

            return temp + "-";
        }

        return next_token;
    }
}

class VerbatimJustifier : public LineBasedJustifer
{
public:
    VerbatimJustifier(int MaxLineLength, ifstream &instream, ofstream &outstream);
    virtual ~VerbatimJustifier();

private:
    virtual string justifyLine(const string &line) override;
};

VerbatimJustifier::VerbatimJustifier(int MaxLineLength, ifstream &instream, ofstream &outstream) : LineBasedJustifer(MaxLineLength, instream, outstream) {}

VerbatimJustifier::~VerbatimJustifier() {}

// add extra spaces to the end of the line if necessary
string VerbatimJustifier::justifyLine(const string &line)
{

    int diff = this->MaxLineLength - line.size();
    string spaces = "";
    for (int i = 0; i < diff; i++)
    {
        spaces.append(" ");
    }
    return line + spaces;
}

int main(int argc, char *argv[])
{
    // Use this string (printer to cerr) as the error message if the
    // provided value MaxLineLength is less than Justifier::MinLineLength
    const string maxLineLengthTooShort = "Error, MaxLineLength must be at least " + ::to_string(Justifier::MinLineLength);

    // Use this string (printed to cerr) as the error message if there are
    // any *other* problems with the command line arguments
    // i.e., we won't bother with more specialized messages that
    // distinguish between different problems
    const string usageMsg = "Usage: justify [-m <MaxLineLen>] [-s [rr|rl|c|s|v]] <infile> <outfile>";

    // First check that there were at least three arguments
    if (argc < 3)
    {
        cerr << usageMsg << endl;
        exit(1);
    }
    if (argc > 7) // handle too many arguments
    {
        cerr << usageMsg << endl;
        exit(1);
    }

    // declare the defaults for cmdline args
    Justifier::Style style = Justifier::Style::RaggedRight;
    int MaxLineLength = Justifier::DefaultMaxLineLength;
    string infile, outfile;

    // Now process the command line args

    // save all the args into a vector for easy access
    vector<string> args{};
    for (int i = 0; i < argc; i++)
    {
        args.push_back(argv[i]);
    }

    // handle no optional arguments
    if (argc == 3)
    {
        infile = args[1];
        outfile = args[2];
    }
    // handle one optional argument
    else if (argc == 5)
    {
        // make sure that the first optional parameter is either "-m" or "-s"
        if (args[1] == "-m")
        {
            int line_length = stoi(args[2]);
            if (line_length < Justifier::MinLineLength)
            {
                cerr << maxLineLengthTooShort << endl;
                exit(1);
            }
            else
            {
                MaxLineLength = line_length;
            }
        }
        else if (args[1] == "-s")
        {
            string style_string = args[2];
            if (style_string == "rr")
            {
                style = Justifier::Style::RaggedRight;
            }
            else if (style_string == "rl")
            {
                style = Justifier::Style::RaggedLeft;
            }
            else if (style_string == "c")
            {
                style = Justifier::Style::Centred;
            }
            else if (style_string == "s")
            {
                style = Justifier::Style::Smooth;
            }
            else if (style_string == "v")
            {
                style = Justifier::Style::Verbatim;
            }
            else
            {
                cerr << usageMsg << endl;
                exit(1);
            }
        }
        else
        {
            cerr << usageMsg << endl;
            exit(1);
        }

        infile = args[3];
        outfile = args[4];
    }
    // handle both optional arguments
    else if (argc == 7)
    {
        // make sure that the first optional parameter is either "-m" and the second optional parameter is "-s"
        if (args[1] == "-m")
        {
            int line_length = stoi(args[2]);
            if (line_length < Justifier::MinLineLength)
            {
                cerr << maxLineLengthTooShort << endl;
                exit(1);
            }
            else
            {
                MaxLineLength = line_length;
            }
        }
        else
        {
            cerr << usageMsg << endl;
            exit(1);
        }

        if (args[3] == "-s")
        {
            string style_string = args[4];
            if (style_string == "rr")
            {
                style = Justifier::Style::RaggedRight;
            }
            else if (style_string == "rl")
            {
                style = Justifier::Style::RaggedLeft;
            }
            else if (style_string == "c")
            {
                style = Justifier::Style::Centred;
            }
            else if (style_string == "s")
            {
                style = Justifier::Style::Smooth;
            }
            else if (style_string == "v")
            {
                style = Justifier::Style::Verbatim;
            }
            else
            {
                cerr << usageMsg << endl;
                exit(1);
            }
        }
        else
        {
            cerr << usageMsg << endl;
            exit(1);
        }

        infile = args[5];
        outfile = args[6];
    }
    else
    {
        cerr << usageMsg << endl;
        exit(1);
    }

    ifstream in{infile};
    ofstream out{outfile};

    // check to make sure input file is valid
    if (!in)
    {
        cerr << usageMsg << endl;
        exit(1);
    }

    // Now we've finished processing the command line args; we print a
    // simple diagnostic message to cerr and then proceed to justification
    cerr << "About to perform justification:" << endl;
    cerr << "    Style         = " << Justifier::StyleName.at(style)
         //  << " (val = " << style << ")"
         << endl;
    cerr << "    MaxLineLength = " << MaxLineLength << endl;
    cerr << "    Input file    = " << infile << endl;
    cerr << "    Output file   = " << outfile << endl;

    // Now create the justifier and do the justification!
    Justifier *justifier;

    switch (style)
    {
    // Instantiate the appropriate concrete Justifier
    case Justifier::RaggedRight:
        justifier = new RaggedRightJustifier{MaxLineLength, in, out};
        break;
    case Justifier::RaggedLeft:
        justifier = new RaggedLeftJustifier{MaxLineLength, in, out};
        break;
    case Justifier::Centred:
        justifier = new CentredJustifier{MaxLineLength, in, out};
        break;
    case Justifier::Smooth:
        justifier = new SmoothJustifier{MaxLineLength, in, out};
        break;
    case Justifier::Verbatim:
        justifier = new VerbatimJustifier{MaxLineLength, in, out};
        break;
    }

    justifier->justify();

    // close in/output file streams
    in.close();
    out.close();

    // clean up memory leaks
    delete justifier;
}