#include "ntt.hpp"
#include "common.hpp" 

const int MOD = 998244353;
const int G = 3;

long long modpow(long long a, long long b) {
    long long res = 1;
    a %= MOD;
    if (a < 0) a += MOD;
    while (b > 0) {
        if (b & 1) res = (res * a) % MOD;
        a = (a * a) % MOD;
        b >>= 1;
    }
    return res;
}

long long modInverse(long long n) {
    return modpow(n, MOD - 2);
}

void ntt(std::vector<long long>& a, bool invert) {
    int n = a.size();
    
    bit_reversal<long long>(a); 

    for (int len = 2; len <= n; len <<= 1) {
        long long wlen = modpow(G, (MOD - 1) / len);
        if (invert) {
            wlen = modInverse(wlen);
        }
        
        for (int i = 0; i < n; i += len) {
            long long w = 1;
            for (int j = 0; j < len / 2; j++) {
                long long u = a[i + j];
                long long v = (a[i + j + len / 2] * w) % MOD;
                
                a[i + j] = (u + v) % MOD;
                a[i + j + len / 2] = (u - v + MOD) % MOD;
                w = (w * wlen) % MOD;
            }
        }
    }

    if (invert) {
        long long n_inv = modInverse(n);
        for (long long& x : a) {
            x = (x * n_inv) % MOD;
        }
    }
}

std::vector<long long> multiply_ntt(std::vector<long long> a, const std::vector<long long>& b) {
    if (a.empty() || b.empty()) return {};

    int exact_size = a.size() + b.size() - 1;
    
    int n = 1;
    while (n < exact_size) {
        n <<= 1;
    }
    
    a.resize(n, 0);
    std::vector<long long> b_copy = b;
    b_copy.resize(n, 0);
    
    ntt(a, false);
    ntt(b_copy, false);
    
    for (int i = 0; i < n; i++) {
        a[i] = (a[i] * b_copy[i]) % MOD;
    }
    
    ntt(a, true);
    
    a.resize(exact_size);
    
    while (a.size() > 1 && a.back() == 0) {
        a.pop_back();
    }
    
    return a;
}