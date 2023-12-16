#include <iostream>
#include <fstream>
#include <iomanip>
#include <cctype>
#include <algorithm>
#include <string>
#include <vector>

#include <cassert>

using namespace std;

int scrabbleValue(char chr)
{
    chr = tolower(chr);

    if ((chr == 'e') || (chr == 'a') || (chr == 'i') || (chr == 'o') || (chr == 'n') || (chr == 'r') || (chr == 't') || (chr == 'l') || (chr == 's') || (chr == 'u'))
    {
        return 1;
    }
    else if ((chr == 'd') || (chr == 'g'))
    {
        return 2;
    }
    else if ((chr == 'b') || (chr == 'c') || (chr == 'm') || (chr == 'p'))
    {
        return 3;
    }
    else if ((chr == 'f') || (chr == 'h') || (chr == 'v') || (chr == 'w') || (chr == 'y'))
    {
        return 4;
    }
    else if ((chr == 'k'))
    {
        return 5;
    }
    else if ((chr == 'j') || (chr == 'x'))
    {
        return 8;
    }
    else if ((chr == 'q') || (chr == 'z'))
    {
        return 10;
    }
    else
    {
        return 0;
    }
}

int scrabbleValue(string str)
{

    int sum = 0;

    for (int i = 0; i < str.length(); i++)
    {
        char temp = str[i];

        sum += scrabbleValue(temp);
    }

    return sum;
}

// Passive data node for hash tables.
struct Node
{
    string word;
    Node *next;
};

class HashTable
{
public:
    HashTable();
    HashTable(int K);
    virtual ~HashTable();
    void insert(string word);
    void remove(string word); // You implement this!
    bool lookup(string word) const;
    void print() const;
    void report() const;
    static const int DefaultSize;

protected:
    int getTableSize() const;

private:
    vector<Node *> table;
    // The "hash" function will be defined by each child class,
    // each one using a different strategy.  But it needs to be
    // abstract (aka "pure virtual") in the abstract parent class.
    virtual int hash(string key) const = 0;
};

// Note that insert/lookup are completely defined here (and call hash) even
// tho hash is not defined here.  This is called the Template Method design
// pattern.  The hash function is private, meaning the children can't call
// it.  However, they can *define* it.  In fact, that's the only difference
// between the children classes: they each provide a difference
// implementation of hash!

const int HashTable::DefaultSize = 1000;
HashTable::HashTable() : table(DefaultSize) {}
HashTable::HashTable(int K) : table(K) {}

// The destructor has lots of garbage to clean up!  For each bucket with an
// overflow list, we delete the nodes.
HashTable::~HashTable()
{
    for (int i = 0; i < getTableSize(); i++)
    {
        Node *p = table.at(i);
        while (nullptr != p)
        {
            Node *temp = p;
            p = p->next;
            delete temp;
        }
    }
}

// Simple accessor function that will be used by children classes to
// implement the hash function.  It's protected as children need to use it
// but not public as external clients do not need to know about it.
// This is the only information about the table that the children classes
// need to know, so we can make the table itself private!
int HashTable::getTableSize() const
{
    return (int)table.size();
}

// Use open hashing with unsorted linked list for overflow.
void HashTable::insert(string key)
{
    const int slot = hash(key);
    Node *newNode = new Node;
    newNode->word = key;
    newNode->next = table.at(slot);
    table.at(slot) = newNode;
}

bool HashTable::lookup(string key) const
{
    const int slot = hash(key);
    Node *curNode = table.at(slot);
    while (nullptr != curNode)
    {
        if (curNode->word == key)
        {
            return true;
        }
        curNode = curNode->next;
    }
    return false;
}

// You implement this!
void HashTable::remove(string key)
{
    if (this->lookup(key))
    {
        const int slot = hash(key);

        Node *curNode = table.at(slot);

        if (curNode->next == nullptr)
        {
            delete curNode;
            table.at(slot) = nullptr;
            return;
        }
        else
        {
            if (curNode->word == key)
            {
                Node *temp = curNode;

                table.at(slot) = curNode->next;
                delete temp;
                return;
            }
            else
            {
                Node *prevNode = curNode;
                curNode = curNode->next;

                while (curNode != nullptr)
                {
                    if (curNode->word == key)
                    {
                        Node *temp = curNode;
                        prevNode->next = curNode->next;

                        delete temp;
                        return;
                    }

                    prevNode = prevNode->next;
                    curNode = curNode->next;
                }
            }
        }
    }
}

// To help you debug, if you find it useful.
void HashTable::print() const
{
    for (int i = 0; i < getTableSize(); i++)
    {
        if (nullptr != table.at(i))
        {
            Node *p = table.at(i);
            while (nullptr != p)
            {
                cout << i << "    " << p->word << endl;
                p = p->next;
            }
        }
    }
}

// So we can tell how good your hash function is ...
// Do NOT override this function in the version you submit.  We will check!
void HashTable::report() const
{
    // K is number of buckets
    const int K = getTableSize();
    if (K == 0)
    {
        cout << "Hash table is empty; nothing to report." << endl;
        return;
    }
    // How many overflow elements in each bucket?
    vector<int> stats(K);
    int totalNumEntries = 0;
    int numNonZeros = 0;
    int maxOverflowSize = 0;
    for (int i = 0; i < K; i++)
    {
        if (nullptr != table.at(i))
        {
            numNonZeros++;
            int numEntriesInThisBucket = 0;
            Node *p = table.at(i);
            while (nullptr != p)
            {
                p = p->next;
                numEntriesInThisBucket++;
            }
            totalNumEntries += numEntriesInThisBucket;
            if (numEntriesInThisBucket > maxOverflowSize)
            {
                maxOverflowSize = numEntriesInThisBucket;
            }
            stats[i] = numEntriesInThisBucket;
        }
    }
    // This part added 28 March 2014:
    // Compute average # of probes required to establish that an element
    // is not present by summing the squares of the bucket list lengths,
    // then dividing by totalNumEntries
    int sumOfSquaresOfBucketListLengths = 0;
    for (int i = 0; i < stats.size(); i++)
    {
        sumOfSquaresOfBucketListLengths += stats.at(i) * stats.at(i);
    }
    const double avgNumberOfProbes = (double)sumOfSquaresOfBucketListLengths / (double)totalNumEntries;
    cout << "Average # of probes to establish an element is not present: "
         << setprecision(3) << fixed << avgNumberOfProbes << endl;

    // If we sort, it's trivial to find the median.
    sort(stats.begin(), stats.end());
    const int numEmptyBuckets = K - numNonZeros;
    const int firstNonZeroBucketIndex = numEmptyBuckets;
    cout << "Number of entries in table: " << totalNumEntries << endl;
    cout << "Total # buckets: " << K << " of which " << numEmptyBuckets
         << " (" << 100 * numEmptyBuckets / K << "%)"
         << " were empty." << endl;
    // We want the avg and median # of elements, but ignoring the empty
    // buckets, on the grounds that if we're looking for a valid word, it
    // doesn't matter how many empty buckets there are, as we'll be hashing
    // to a non-empty bucket and traversing that list.
    // To compute the median, find the index that is half-way between
    // firstNonZeroBucketIndex and K.
    const int median = stats[firstNonZeroBucketIndex + numNonZeros / 2];
    const int average = totalNumEntries / numNonZeros;
    cout << "Overflow list length:  Max = " << maxOverflowSize
         << "  Median = " << median << "  Average = " << average << endl;
}

class SmartHashTable : public HashTable
{
public:
    SmartHashTable();
    SmartHashTable(int K);
    ~SmartHashTable();

private:
    int hash(string key) const;
};

SmartHashTable::SmartHashTable() : HashTable() {}
SmartHashTable::SmartHashTable(int K) : HashTable(K) {}
SmartHashTable::~SmartHashTable() {}

int SmartHashTable::hash(string key) const
{

    // code adapted from https://stackoverflow.com/a/8317622
    int h = 37;

    for (int i = 0; i < key.length(); i++)
    {
        int character = key[i];

        h = (h * 54059) ^ (character * 76963);
    }
    //

    int mod = h % this->getTableSize();

    if (mod < 0)
    {
        mod *= -1;
    }

    return mod;
}

// Ancillary function for powerset.  It adds a character onto the beginning
// of each string in a vector.
vector<string> addChar(const vector<string> &v, char c)
{
    vector<string> ans;
    for (int i = 0; i < (int)v.size(); i++)
    {
        ans.push_back(c + v.at(i));
    }
    return ans;
}

// Note that I wrote this in scheme first!  Programming "shell game"
// recursion puzzles is easier with weak typing.
// This takes a string and returns the power(multi)set of its characters
// as a vector.  For example, powerset of "aab" would be the vector with
// elements (in no particular order): "aab" "ab" "aa" "a" "ab" "a" "b".
// Assume we don't care about eliminating duplicates for now.
vector<string> powerset(string s)
{
    vector<string> ans;
    if (s.size() == 0)
    {
        ans.push_back("");
    }
    else
    {
        char c = s.at(0);
        string rest = s.substr(1);
        vector<string> psetRest = powerset(rest);
        ans = addChar(psetRest, c);
        ans.insert(ans.end(), psetRest.begin(), psetRest.end());
    }
    return ans;
}

int main(int argc, char *argv[])
{

    if (argc != 2)
    {
        cerr << "Error, no word list file name provided." << endl;
        exit(1);
    }

    ifstream file{argv[1]};

    if (!file)
    {
        cout << "Error, couldn't open word list file." << endl;
        exit(1);
    }

    SmartHashTable map{1000000};

    string current_word;

    while (file >> current_word)
    {
        map.insert(current_word);
    }

    vector<string> words{};
    while (cin >> current_word)
    {
        words.push_back(current_word);
    }

    for (auto word : words)
    {
        vector<string> permutations = powerset(word);

        int max_score = 0;
        string max_permutation;

        for (auto permutation : permutations)
        {
            string s = permutation;

            sort(s.begin(), s.end());

            do
            {
                if (map.lookup(s))
                {
                    int score = scrabbleValue(s);

                    if (score > max_score)
                    {
                        max_score = score;
                        max_permutation = s;
                    }
                }

            } while (next_permutation(s.begin(), s.end()));
        }

        if (max_score == 0)
        {
            cout << word << ": no matches" << endl;
        }
        else
        {
            cout << word << ": " << max_permutation << " has score of " << max_score << endl;
        }
    }

    return 0;
}