#pragma once
#include <vector>
#include <algorithm>

constexpr double PI = 3.14159265358979323846;

template <typename T>
void bit_reversal(std::vector<T>& a) {
    int n = a.size();
    for (int i = 1, j = 0; i < n; i++) {
        int bit = n >> 1;
        for (; j & bit; bit >>= 1) {
            j ^= bit;
        }
        j ^= bit;
        if (i < j) {
            std::swap(a[i], a[j]);
        }
    }
}