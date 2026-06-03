#include "baselines.hpp"
#include <algorithm>

using namespace std;

vector<cd> naive_multiply(const vector<cd>& a, const vector<cd>& b) {
    int n = a.size();
    int m = b.size();
    vector<cd> res(n + m - 1, 0);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            res[i + j] += a[i] * b[j];
        }
    }
    return res;
}

vector<cd> karatsuba_recursive(const vector<cd>& a, const vector<cd>& b) {
    int n = a.size();
    if (n <= 16) {
        return naive_multiply(a, b);
    }

    int k = n / 2;
    vector<cd> a0(a.begin(), a.begin() + k);
    vector<cd> a1(a.begin() + k, a.end());
    vector<cd> b0(b.begin(), b.begin() + k);
    vector<cd> b1(b.begin() + k, b.end());

    vector<cd> a01(k), b01(k);
    for (int i = 0; i < k; i++) {
        a01[i] = a0[i] + a1[i];
        b01[i] = b0[i] + b1[i];
    }

    auto c0 = karatsuba_recursive(a0, b0);
    auto c2 = karatsuba_recursive(a1, b1);
    auto c1 = karatsuba_recursive(a01, b01);

    vector<cd> res(2 * n - 1, 0);
    for (int i = 0; i < 2 * k - 1; i++) {
        res[i] += c0[i];
        res[i + k] -= c0[i];
        res[i + k] += c1[i];
        res[i + k] -= c2[i];
        res[i + 2 * k] += c2[i];
    }
    return res;
}

vector<cd> karatsuba_multiply(vector<cd> a, vector<cd> b) {
    int n = 1;
    while (n < max(a.size(), b.size())) n <<= 1;
    a.resize(n, 0);
    b.resize(n, 0);
    return karatsuba_recursive(a, b);
}