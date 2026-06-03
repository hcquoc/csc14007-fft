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