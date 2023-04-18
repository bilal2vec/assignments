#include <iostream>
#include <vector>
#include <map>

using namespace std;

int N;
int M;
int C;

vector<int> T;
vector<int> R;
vector<int> D;

// city, money, fuel -> time
map<tuple<int, int, int>, tuple<int, vector<int>>> dp;

int max_time = 1000000;

// current_city, current_money, current_fuel -> best time left until end
tuple<int, vector<int>> opt(int city, int money, int fuel)
{
    if (city == N - 1)
    {
        return make_tuple(0, vector<int>(N, 0));
    }

    if (dp.find(make_tuple(city, money, fuel)) != dp.end())
    {
        return dp[make_tuple(city, money, fuel)];
    }

    int best_time_to_end = max_time;
    vector<int> best_fuel_to_buy(N, 0);

    int max_fuel_to_buy = min(C - fuel, money / D[city]);

    int time_to_next_city = T[city];

    for (int fuel_to_buy = 0; fuel_to_buy <= max_fuel_to_buy; fuel_to_buy++)
    {
        int money_prime = money - fuel_to_buy * D[city];
        int fuel_prime = fuel + fuel_to_buy - time_to_next_city;
        int fueling_time = fuel_to_buy * R[city];

        if (fuel_prime >= 0)
        {
            tuple<int, vector<int>> next_city_opt = opt(city + 1, money_prime, fuel_prime);
            int best_time_left_to_end_from_next_city = get<0>(next_city_opt);

            vector<int> best_fuel_added_from_next_city = get<1>(next_city_opt);
            best_fuel_added_from_next_city[city] = fuel_to_buy;

            int curr_time_to_end = fueling_time + time_to_next_city + best_time_left_to_end_from_next_city;

            if (curr_time_to_end < best_time_to_end)
            {
                best_time_to_end = curr_time_to_end;
                best_fuel_to_buy = best_fuel_added_from_next_city;
            }
        }
    }

    dp[make_tuple(city, money, fuel)] = make_tuple(best_time_to_end, best_fuel_to_buy);

    return make_tuple(best_time_to_end, best_fuel_to_buy);
}

int main()
{
    cin >> N >> M >> C;

    for (int i = 0; i < N - 1; i++)
    {
        int t;
        cin >> t;
        T.push_back(t);
    }

    for (int i = 0; i < N; i++)
    {
        int t;
        cin >> t;
        R.push_back(t);
    }

    for (int i = 0; i < N; i++)
    {
        int t;
        cin >> t;
        D.push_back(t);
    }

    tuple<int, vector<int>> sol = opt(0, M, C);
    int best_time = get<0>(sol);
    vector<int> amount_of_fuel_added = get<1>(sol);

    if (best_time == max_time)
    {
        cout << "-1" << endl;
        return 0;
    }

    cout << best_time << endl;

    for (int i = 0; i < N; i++)
    {
        cout << amount_of_fuel_added[i] << " ";
    }

    cout << endl;

    return 0;
}