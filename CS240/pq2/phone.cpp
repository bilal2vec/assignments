#include <iostream>
#include <string>
#include <vector>
#include <cmath>

using namespace std;

int find_first_bigger_prime(int start)
{
    // pre: start > 10
    // post: returns smallest prime that is at least start
    if (start % 2 == 0)
    {
        start++;
    } // no need to test even numbers
    for (int i = start; i <= (2 * start); i += 2)
    {
        // By Bertrand's postulate, a prime has to exist in (start, 2*start)
        bool isPrime = true;
        for (int j = 3; j * j <= i; j += 2)
        {
            if (i % j == 0)
            {
                isPrime = false;
                break;
            }
        }
        if (isPrime == true)
        {
            return i;
        }
    }
}

int hash0(long key, int tableSize)
{
    return (int)(key % tableSize);
}

int hash1(long key, int tableSize)
{
    double phi = (sqrt(5) - 1) / 2;
    double val = key * phi;
    return (int)floor(tableSize * (val - floor(val)));
}

// Start of Hashing with Chaining //

struct Node
{
    string key;
    string value;
    Node *next;

    Node() : next{nullptr} {}

    Node(string key, string value) : key{key}, value{value}, next{nullptr} {}

    Node(string key, string value, Node *next) : key{key}, value{value}, next{next} {}

    string getKey()
    {
        return key;
    }

    string getValue()
    {
        return value;
    }

    Node *getNext()
    {
        return next;
    }

    ~Node()
    {
        if (next != nullptr)
        {
            delete next;
        }
    }
};

struct ChainingHashing
{
    int M;
    vector<Node *> table;

    ChainingHashing() : M{11}, table(M, nullptr) {}

    int hash(string key)
    {
        int R = 255;
        long result = key[0];
        for (int i = 1; i < key.length(); ++i)
        {
            char c = key[i];
            result = hash0(result * R + c, M);
        }
        return hash0(result, M);
    }

    // Insert for Hashing with Chaining //
    void insert(string key, string value)
    {
        int hashed_key = hash(key);

        if (table[hashed_key] == nullptr)
        {
            table[hashed_key] = new Node(key, value);
            return;
        }
        else
        {
            if (table[hashed_key]->getValue() > value)
            {
                table[hashed_key] = new Node(key, value, table[hashed_key]);
                return;
            }

            Node *curr = table[hashed_key];
            while (curr->getNext() != nullptr && curr->getNext()->getValue() < value)
            {
                curr = curr->getNext();
            }

            curr->next = new Node(key, value, curr->getNext());
            return;
        }
    }

    // Searching for Hashing with Chaining //
    void search(string key)
    {
        int hashed_key = hash(key);
        Node *curr = table[hashed_key];

        if (curr == nullptr)
        {
            cout << "not found" << endl;
            return;
        }
        else
        {
            while (curr != nullptr)
            {
                if (curr->getKey() == key)
                {
                    cout << curr->getValue() << " ";
                }

                curr = curr->getNext();
            }
            cout << endl;
            return;
        }
    }

    // Rehashing for Hashing with Chaining //
    void rehash()
    {
        int old_M = M;
        vector<Node *> old_table = table;

        M = find_first_bigger_prime(2 * M + 1);
        table = vector<Node *>(M, nullptr);

        for (int i = 0; i < old_M; ++i)
        {
            Node *curr = old_table[i];
            while (curr != nullptr)
            {
                insert(curr->getKey(), curr->getValue());
                curr = curr->getNext();
            }
            delete old_table[i];
        }
    }

    // Print for Hashing with Chaining //
    void print()
    {
        cout << M;
        for (int i = 0; i < M; ++i)
        {
            int n_nodes = 0;
            Node *curr = table[i];
            while (curr != nullptr)
            {
                n_nodes += 1;
                curr = curr->getNext();
            }
            cout << " " << n_nodes;
        }
        cout << endl;
    }

    // Reset for Hashing with Chaining //
    void reset()
    {
        for (int i = 0; i < M; ++i)
        {
            if (table[i] != nullptr)
            {
                delete table[i];
                table[i] = nullptr;
            }
        }

        M = 11;
        table = std::vector<Node *>(M, nullptr);
    }

    ~ChainingHashing()
    {
        for (int i = 0; i < M; ++i)
        {
            if (table[i] != nullptr)
            {
                delete table[i];
            }
        }
    }
};

// End of Hashing with Chaining //

// Start of Cuckoo Hashing //

struct CuckooHashing
{
    int M;
    vector<pair<string, string>> t_1;
    vector<pair<string, string>> t_2;

    long convert_number(string s)
    {
        string new_s;
        for (int i = 0; i < s.size(); ++i)
        {
            char c = s[i];
            if (isdigit(c))
            {
                new_s += c;
            }
        }
        return stol(new_s);
    }

    CuckooHashing() : M{11}, t_1(M), t_2(M) {}

    // Insert for Cuckoo Hashing //
    void insert(string key, string value)
    {
        pair<string, string> kv = make_pair(key, value);

        int i = 0;
        int count = 0;
        while (true)
        {
            if (count == M)
            {
                cout << "cannot insert" << endl;
                break;
            }

            if (i == 0)
            {
                if (t_1[hash0(convert_number(kv.first), M)].first.empty())
                {
                    t_1[hash0(convert_number(kv.first), M)] = kv;
                    break;
                }
                else
                {
                    pair<string, string> temp = t_1[hash0(convert_number(kv.first), M)];
                    t_1[hash0(convert_number(kv.first), M)] = kv;
                    kv = temp;

                    i = 1 - i;
                }
            }
            else
            {
                if (t_2[hash1(convert_number(kv.first), M)].first.empty())
                {
                    t_2[hash1(convert_number(kv.first), M)] = kv;
                    break;
                }
                else
                {
                    pair<string, string> temp = t_2[hash1(convert_number(kv.first), M)];
                    t_2[hash1(convert_number(kv.first), M)] = kv;
                    kv = temp;

                    i = 1 - i;
                }
            }
            count += 1;
        }
    }

    // Searching for Cuckoo Hashing //
    void search(string key)
    {
        long processed_key = convert_number(key);

        int key_0 = hash0(processed_key, M);
        int key_1 = hash1(processed_key, M);

        if (t_1[key_0].first == key)
        {
            cout << t_1[key_0].second << endl;
        }
        else if (t_2[key_1].first == key)
        {
            cout << t_2[key_1].second << endl;
        }
        else
        {
            cout << "not found" << endl;
        }
    }

    // Rehashing for Cuckoo Hashing //
    void rehash()
    {
        int old_M = M;
        vector<pair<string, string>> old_t_1 = t_1;
        vector<pair<string, string>> old_t_2 = t_2;

        M = find_first_bigger_prime(2 * M + 1);
        t_1 = vector<pair<string, string>>(M);
        t_2 = vector<pair<string, string>>(M);

        for (int i = 0; i < old_M; ++i)
        {
            pair<string, string> curr = old_t_1[i];
            if (curr.first != "")
            {
                insert(curr.first, curr.second);
            }
        }

        for (int i = 0; i < old_M; ++i)
        {
            pair<string, string> curr = old_t_2[i];
            if (curr.first != "")
            {
                insert(curr.first, curr.second);
            }
        }
    }

    // Print for Cuckoo Hashing //
    void print()
    {
        cout << M;

        for (int i = 0; i < M; ++i)
        {
            if (t_1[i].second.size() == 0)
            {
                cout << " " << 0;
            }
            else
            {
                cout << " " << 1;
            }
        }

        for (int i = 0; i < M; ++i)
        {
            if (t_2[i].second.size() == 0)
            {
                cout << " " << 0;
            }
            else
            {
                cout << " " << 1;
            }
        }
        cout << endl;
    }

    // Reset for Cuckoo Hashing //
    void reset()
    {
        M = 11;
        t_1 = vector<pair<string, string>>(M);
        t_2 = vector<pair<string, string>>(M);
    }

    ~CuckooHashing() {}
};

// End of Cuckoo Hashing //

int main()
{

    ChainingHashing d_name;
    CuckooHashing d_phone;

    string cmd;

    while (cin >> cmd)
    {
        if (cmd == "i")
        {
            string name;
            string number;

            cin >> name >> number;

            d_name.insert(name, number);
            d_phone.insert(number, name);
        }
        else if (cmd == "l")
        {
            string number;
            cin >> number;

            d_phone.search(number);
        }
        else if (cmd == "s")
        {
            string name;
            cin >> name;

            d_name.search(name);
        }
        else if (cmd == "rh")
        {
            string dict_num_string;
            int dict_num;
            cin >> dict_num_string;
            dict_num = stoi(dict_num_string);

            if (dict_num == 0)
            {
                d_name.rehash();
            }
            else if (dict_num == 1)
            {
                d_phone.rehash();
            }
        }
        else if (cmd == "p")
        {
            string dict_num_string;
            int dict_num;
            cin >> dict_num_string;
            dict_num = stoi(dict_num_string);

            if (dict_num == 0)
            {
                d_name.print();
            }
            else if (dict_num == 1)
            {
                d_phone.print();
            }
        }
        else if (cmd == "r")
        {
            d_name.reset();
            d_phone.reset();
        }
        else if (cmd == "x")
        {
            break;
        }
        else
        {
            cout << "Invalid command" << endl;
        }
    }

    return 0;
}