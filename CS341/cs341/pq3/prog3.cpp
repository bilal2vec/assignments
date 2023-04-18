#include <iostream>
#include <vector>
#include <tuple>
#include <algorithm>
#include <unordered_map>
#include <map>

using namespace std;

int N;
int M;

vector<tuple<int, int, int, int, int>> activities;

vector<int> nexts;
vector<tuple<int>> children;

map<tuple<int, int>, bool> visited;
map<tuple<int, int>, int> answer;

vector<int> schedule;

bool sort_by_start_time(const tuple<int, int, int, int, int> &a, const tuple<int, int, int, int, int> &b)
{
    return get<0>(a) < get<0>(b);
}

int opt(int i, int e)
{
    if (i >= N || e < 0)
    {
        return 0;
    }

    tuple<int, int> state = make_tuple(i, e);

    if (visited.find(state) != visited.end())
    {
        return answer[state];
    }

    int v_i = get<2>(activities[i]);
    int e_i = get<3>(activities[i]);

    if (i == 0)
    {
        e += get<0>(activities[i]);
    }

    if (e - e_i < 0)
    {
        int energy_increase = get<0>(activities[i + 1]) - get<0>(activities[i]);

        answer[state] = opt(i + 1, e + energy_increase);
        visited[state] = true;

        return answer[state];
    }
    else
    {
        // between end of interval i and start of interval nexts[i]
        int energy_increase_1 = get<0>(activities[nexts[i]]) - get<1>(activities[i]);

        // between start of interval i and start of interval i + 1
        int energy_increase_2 = get<0>(activities[i + 1]) - get<0>(activities[i]);

        if (v_i + opt(nexts[i], e - e_i + energy_increase_1) > opt(i + 1, e + energy_increase_2))
        {
            answer[state] = v_i + opt(nexts[i], e - e_i + energy_increase_1);
        }
        else
        {
            answer[state] = opt(i + 1, e + energy_increase_2);
        }

        visited[state] = true;

        return answer[state];
    }
}

void get_schedule(int i, int e)
{
    if (i >= N || e < 0)
    {
        return;
    }

    int v_i = get<2>(activities[i]);
    int e_i = get<3>(activities[i]);

    if (i == 0)
    {
        e += get<0>(activities[i]);
    }

    if (e - e_i < 0)
    {
        int energy_increase = get<0>(activities[i + 1]) - get<0>(activities[i]);

        get_schedule(i + 1, e + energy_increase);
    }
    else
    {
        // between end of interval i and start of interval nexts[i]
        int energy_increase_1 = get<0>(activities[nexts[i]]) - get<1>(activities[i]);

        // between start of interval i and start of interval i + 1
        int energy_increase_2 = get<0>(activities[i + 1]) - get<0>(activities[i]);

        if (v_i + opt(nexts[i], e - e_i + energy_increase_1) > opt(i + 1, e + energy_increase_2))
        {
            int id = get<4>(activities[i]);

            schedule.push_back(id);

            get_schedule(nexts[i], e - e_i + energy_increase_1);
        }
        else
        {
            get_schedule(i + 1, e + energy_increase_2);
        }
    }
}

int main()
{
    cin >> N >> M;

    for (int i = 0; i < N; ++i)
    {
        int S_i, T_i, V_i, E_i;

        cin >> S_i >> T_i >> V_i >> E_i;

        activities.push_back(make_tuple(S_i, T_i, V_i, E_i, i + 1));
    }

    sort(activities.begin(), activities.end(), sort_by_start_time);

    // cout << "---" << endl;

    // for (int i = 0; i < N; ++i)
    // {
    //     cout << get<0>(activities[i]) << " " << get<1>(activities[i]) << " " << get<2>(activities[i]) << " " << get<3>(activities[i]) << " " << get<4>(activities[i]) << endl;
    // }

    // cout << "---" << endl;

    nexts = vector<int>(N, 0);

    for (int i = 0; i < N; ++i)
    {
        int s_i = get<0>(activities[i]);
        int f_i = get<1>(activities[i]);

        nexts[i] = N;

        for (int j = i + 1; j < N; ++j)
        {
            int s_j = get<0>(activities[j]);

            if (s_j >= f_i)
            {
                nexts[i] = j;
                break;
            }
        }
    }

    opt(0, M);
    get_schedule(0, M);

    int K = schedule.size();
    int V = answer[make_tuple(0, M)];

    cout << K << " " << V << endl;

    for (int i = 0; i < schedule.size(); ++i)
    {
        cout << schedule[i] << " ";
    }

    cout << endl;

    return 0;
}