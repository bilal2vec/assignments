#include <iostream>
#include <fstream>
#include <string>
#include <cassert>
#include <vector>

using namespace std;

int main(int argc, char *argv[])
{

    int N;
    cin >> N;

    if (N < 1)
    {
        cerr << "Error, line length must be positive." << endl;
        exit(1);
    }

    string textFileName;

    cin >> textFileName;

    ifstream file{textFileName};

    if (!file)
    {
        cerr << "Error, cannot open specified text file." << endl;
        exit(1);
    }

    vector<string> characters{};
    string character;
    while (file >> character)
    {
        characters.push_back(character);
    }

    file.close();

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

    string current_command;
    string c1;
    string c2;

    while (cin >> current_command)
    {
        if ((current_command != "rr") && (current_command != "rl") && (current_command != "c") && (current_command != "j") && (current_command != "f") && (current_command != "r") && (current_command != "p") && (current_command != "k") && (current_command != "s") && (current_command != "q"))
        {
            cerr << "Error, command is illegal." << endl;
            exit(1);
        }

        if ((current_command == "rr") || (current_command == "rl") || (current_command == "c") || (current_command == "j"))
        {
            c1 = current_command;
        }

        if ((current_command == "f") || (current_command == "r"))
        {
            c2 = current_command;
        }

        if ((current_command == "p") || (current_command == "k") || (current_command == "s"))
        {
            vector<string> out;

            int counter = 0;
            for (auto line : lines)
            {

                vector<int> spaces;

                int K = N;
                if (c1 == "j")
                {
                    int J = line.size() - 1;

                    for (int len : line)
                    {
                        K -= len;
                    }
                    K -= J;

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
                }

                if (c1 == "rl")
                {
                    int J = line.size() - 1;

                    for (int len : line)
                    {
                        K -= len;
                    }
                    K -= J;
                }

                int left_pad = 0;
                int right_pad = 0;

                if (c1 == "c")
                {
                    int J = line.size() - 1;

                    for (int len : line)
                    {
                        K -= len;
                    }
                    K -= J;

                    if (K % 2 == 0)
                    {
                        left_pad = K / 2;
                        right_pad = K / 2;
                    }
                    else
                    {
                        left_pad = (K - 1) / 2;
                        right_pad = (K - 1) / 2;
                        left_pad += 1;
                    }
                }

                string out_line = "";

                if (c1 == "rl")
                {
                    for (int tempp = 0; tempp < K; ++tempp)
                    {
                        out_line += " ";
                    }
                }

                if (c1 == "c")
                {
                    for (int tempp = 0; tempp < left_pad; ++tempp)
                    {
                        out_line += " ";
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

                    out_line += token;

                    if (i != line.size() - 1)
                    {
                        out_line += " ";
                    }

                    if (c1 == "j")
                    {
                        if (i < spaces.size())
                        {
                            for (int k = 0; k < spaces[i]; ++k)
                            {
                                out_line += " ";
                            }
                        }
                    }

                    counter += 1;
                }

                if (c1 == "c")
                {
                    for (int tempp = 0; tempp < right_pad; ++tempp)
                    {
                        out_line += " ";
                    }
                }

                out_line += "\n";
                out.push_back(out_line);
            }

            if (c2 == "r")
            {
                vector<string> rev_out;
                for (int i = out.size() - 1; i >= 0; --i)
                {
                    rev_out.push_back(out[i]);
                }
                out = rev_out;
            }

            if (current_command == "p")
            {
                for (auto line : out)
                {
                    cout << line;
                }
            }

            if ((current_command == "k"))
            {
                int num;
                cin >> num;

                if (num < 0)
                {
                    cerr << "Error, line length must be positive." << endl;
                    exit(1);
                }

                if (num < out.size())
                {
                    cout << out[num];
                }
            }

            if ((current_command == "s"))
            {
                string search;
                cin >> search;

                vector<string> out2;
                for (int i = out.size() - 1; i >= 0; --i)
                {
                    if (out[i].find(search, 0) != -1)
                    {
                        out2.push_back(out[i]);
                    }
                }

                for (auto line : out2)
                {
                    cout << line;
                }
            }
        }

        if (current_command == "q")
        {
            break;
        }
    }

    return 0;
}