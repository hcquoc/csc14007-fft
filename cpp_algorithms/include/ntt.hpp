#pragma once
#include <vector>

extern const int MOD;
extern const int G;

long long modpow(long long a, long long b);

long long modInverse(long long n);

void ntt(std::vector<long long>& a, bool invert);

std::vector<long long> multiply_ntt(std::vector<long long> a, const std::vector<long long>& b);