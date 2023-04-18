#include <iostream>
#include <cmath>

using namespace std;

long* new_arr(int n) {
    long* ptr = (long*) malloc(sizeof(long) * n);
    
    for (int i = 0; i < n; ++i) {
        ptr[i] = 0;
    }

    return ptr;
}

long* karatsuba(long* f, long* g, int n) {
    if (n == 1) {
        long* ptr = new_arr(1);
        ptr[0] = f[0] * g[0];

        return ptr;
    } 

    int n2 = n / 2;

    long* f0 = f;
    long* g0 = g;
    long* f1 = f + n2;
    long* g1 = g + n2;

    long* f0g0 = karatsuba(f0, g0, n2);
    long* f1g1 = karatsuba(f1, g1, n2);
 
    long* h = new_arr(2 * n - 1);
    
    for (int i = 0; i < n - 1; ++i) {
        h[i] += f0g0[i];
    }

    for (int i = 0; i < n - 1; ++i) {
        h[i + n] += f1g1[i];
    }

    long* f0f1 = new_arr(n2);
    long* g0g1 = new_arr(n2);

    for (int i = 0; i < n2; ++i) {
        f0f1[i] = f0[i] + f1[i];
        g0g1[i] = g0[i] + g1[i];
    }

    long* f0f1g0g1 = karatsuba(f0f1, g0g1, n2);

    for (int i = 0; i < n - 1; ++i) {
        h[i + n2] = f0f1g0g1[i] - f0g0[i] - f1g1[i];
    }

    return h;
}

int nearest_power_2(int n) {
    double nd = (double) n;
    double logn = log2(nd);
    double n_ceil = ceil(logn);
    double nearest_pow = pow(2, n_ceil);

    return (int) nearest_pow;
}

int main() {

    int n;
    cin >> n;

    int n_pad = nearest_power_2(n + 1);

    long* f = new_arr(n_pad);
    long* g = new_arr(n_pad);

    for (int i = 0; i < n + 1; ++i) {
        int temp;
        cin >> temp;
        f[i] = temp;
    }

    for (int i = 0; i < n + 1; ++i) {
        int temp;
        cin >> temp;
        g[i] = temp;
    }

    long* h = new_arr(2 * n_pad - 1);


    for (int i = 0; i < n + 1; ++i) {
       for (int j = 0; j < n + 1; ++j) {
            h[i + j] = h[i + j] + (f[i] * g[j]);
        }
    }


    for (int i = 0; i < 2 * n_pad - 1; ++i) {
        cout << h[i] << " ";
    }
    
    cout << endl;

    delete f;
    delete g;
    delete h;

    return 0;
}
