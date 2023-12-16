#include <iostream>
#include <string>
#include <cassert>
#include <vector>

using namespace std;

int main(int argc, char *argv[])
{

    int N;
    cin >> N;

    if (N <= 0)
    {
        cerr << "Error, line length must be positive." << endl;
        exit(1);
    }

    vector<string> characters{};
    string character;
    while (cin >> character)
    {
        characters.push_back(character);
    }

    vector<int> lens{};
    for (int i = 0; i < characters.size(); ++i)
    {
        int len = characters[i].length();

        if (len > N)
        {
            len = N;
        }

        lens.push_back(len);
    }

    vector<vector<int>> lines;
    vector<int> line;
    int length = 0;
    for (auto len : lens)
    {
        if (length == 0)
        {
            line.push_back(len);
            length += len;
        }
        else
        {
            if (length + len + line.size() > N)
            {
                lines.push_back(line);
                line.clear();
                line = {len};
                length = len;
            }
            else if (length + len + line.size() == N)
            {
                line.push_back(len);
                lines.push_back(line);
                line.clear();
                length = 0;
            }
            else
            {
                line.push_back(len);
                length += len;
            }
        }
    }
    if (line.size() > 0)
    {
        lines.push_back(line);
    }

    int counter = 0;
    for (auto line : lines)
    {

        int J = line.size() - 1;

        int K = N;
        for (int len : line)
        {
            K -= len;
        }
        K -= J;

        vector<int> spaces;

        for (int i = 0; i < line.size() - 1; ++i)
        {
            spaces.push_back(0);
        }

        if (spaces.size() <= 0)
        {
            spaces.push_back(K);
        }
        else
        {
            int temp2 = 0;
            while (K > 0)
            {
                if (temp2 == spaces.size())
                {
                    temp2 = 0;
                }
                else
                {
                    spaces[temp2] += 1;
                    K -= 1;
                    temp2 += 1;
                }
            }
        }

        for (int i = 0; i < line.size(); ++i)
        {
            int len = line[i];
            string token = characters[counter];

            if (len != token.length())
            {
                token = token.substr(0, len);
            }

            cout << token;

            if (i != line.size() - 1)
            {
                cout << " ";
            }

            if (i < spaces.size())
            {
                for (int k = 0; k < spaces[i]; ++k)
                {
                    cout << " ";
                }
            }

            counter += 1;
        }
        cout << endl;
    }

    return 0;
}

// 35 1234567890123456789012345678901234567890 Who steals  my purse steals trash; ’tis something, nothing; ’Twas mine,     ’tis his, and has been slave to thousands; fnord But he that filches from me my good name Robs me fnord of     that which not enriches him, And    makes me poor indeed.