#include <iostream>
#include <cmath>
#include <unordered_map>
#include <vector>
#include <queue>

using namespace std;

int hash_fn(int s1, int s2, int s3, int s4) {
    return s1 * (100*100*100) + s2 * (100*100) + s3 * 100 + s4;
}

tuple<int, int, int, int> unhash_fn(int hash) {
    int s1 = hash / (100*100*100);
    hash %= (100*100*100);
    int s2 = hash / (100*100);
    hash %= (100*100);
    int s3 = hash / 100;
    hash %= 100;
    int s4 = hash;

    return make_tuple(s1, s2, s3, s4);
}

void neighbors(vector<int>& next_states, int hash, int n) {
    int s1 = hash / (100*100*100);
    hash %= (100*100*100);
    int s2 = hash / (100*100);
    hash %= (100*100);
    int s3 = hash / 100;
    hash %= 100;
    int s4 = hash;

    next_states.clear();

    // reflecting s1 right
    if (s3 > 2 * s2 - s1) {
        next_states.push_back(hash_fn(s2, 2 * s2 - s1, s3, s4));
    }

    // reflecting s2 right
    if (s4 > 2 * s3 - s2) {
        next_states.push_back(hash_fn(s1, s3, 2 * s3 - s2, s4));
    }

    // reflecting s3 right
    if (n >= 2 * s4 - s3) {
        next_states.push_back(hash_fn(s1, s2, s4, 2 * s4 - s3));
    }

    // reflecting s2 left
    if (1 <= 2 * s1 - s2) {
        next_states.push_back(hash_fn(2 * s1 - s2, s1, s3, s4));
    }

    // reflecting s3 left

    if (s1 < 2 * s2 - s3) {
        next_states.push_back(hash_fn(s1, 2 * s2 - s3, s2, s4));
    }

    // reflecting s4 left

    if (s2 < 2 * s3 - s4) {
        next_states.push_back(hash_fn(s1, s2, 2 * s3 - s4, s3));
    }

    // move s1 left/right
    if (s1 > 1) {
        next_states.push_back(hash_fn(s1 - 1, s2, s3, s4));
    }

    if (s2 > 1 + s1) {
        next_states.push_back(hash_fn(s1 + 1, s2, s3, s4));
    }

    // move s2 left/right
    if (s1 < s2 - 1) {
        next_states.push_back(hash_fn(s1, s2 - 1, s3, s4));
    }

    if (s3 > 1 + s2) {
        next_states.push_back(hash_fn(s1, s2 + 1, s3, s4));
    }

    // move s3 left/right
    if (s2 < s3 - 1) {
        next_states.push_back(hash_fn(s1, s2, s3 - 1, s4));
    }

    if (s4 > 1 + s3) {
        next_states.push_back(hash_fn(s1, s2, s3 + 1, s4));
    }

    // move s4 left/right
    if (s3 < s4 - 1) {
        next_states.push_back(hash_fn(s1, s2, s3, s4 - 1));
    }

    if (n > s4) {
        next_states.push_back(hash_fn(s1, s2, s3, s4 + 1));
    }

}

int main()
{
    int n;
    cin >> n;

    int s1, s2, s3, s4;
    int t1, t2, t3, t4;

    cin >> s1 >> s2 >> s3 >> s4;
    cin >> t1 >> t2 >> t3 >> t4;

    int S = hash_fn(s1, s2, s3, s4);
    int T = hash_fn(t1, t2, t3, t4);

    vector<int> queue;
    queue.reserve(80*80*80*80);
    int start = 0;

    vector<int> next_states = vector<int>(16, 0);

    unordered_map<int, int> visited;
    visited.reserve(n*n*n*n);

    queue.push_back(S);
    visited[S] = -1;

    while (!queue.empty()) {
        int state = queue[start];
        start++;

        if (state == T) {
            break;
        }

        neighbors(next_states, state, n);

        for (auto next_state : next_states) {
            if (visited.find(next_state) == visited.end()) {
                queue.push_back(next_state);
                visited[next_state] = state;
            }
        }

    }

    int path_len = 0;

    int curr_hash = T;
    vector<tuple<int, int, int, int>> path;
    path.reserve(80*80);

    while (curr_hash != -1) {
        path.push_back(unhash_fn(curr_hash));
        path_len++;
        curr_hash = visited[curr_hash];
    }

    cout << path_len - 1 << endl;

    int a, b, c, d;
    for (int i = path_len - 1; i >= 0; i--) {
        tie(a, b, c, d) = path[i];
        cout << a << " " << b << " " << c << " " << d << endl;
    }

    return 0;
}