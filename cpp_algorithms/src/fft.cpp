#include "fft.hpp"
#include "common.hpp" 
#include <cmath>

void fft(std::vector<cd>& a, bool invert) {
    int n = a.size();
    
    bit_reversal<cd>(a); 

    for (int len = 2; len <= n; len <<= 1) {
        double angle = 2 * PI / len * (invert ? -1 : 1);
        cd wlen(std::cos(angle), std::sin(angle));
        
        for (int i = 0; i < n; i += len) {
            cd w(1);
            for (int j = 0; j < len / 2; j++) {
                cd u = a[i + j];
                cd v = a[i + j + len / 2] * w;
                
                a[i + j] = u + v;
                a[i + j + len / 2] = u - v;
                w *= wlen;
            }
        }
    }

    if (invert) {
        for (cd& x : a) x /= n;
    }
}

std::vector<cd> multiply_fft(std::vector<cd> a, std::vector<cd> b) {
    if (a.empty() || b.empty()) return {};

    int exact_size = a.size() + b.size() - 1;
    
    int n = 1;
    while (n < exact_size) {
        n <<= 1;
    }
    
    a.resize(n, 0);
    b.resize(n, 0);
    
    fft(a, false);
    fft(b, false);
    
    for (int i = 0; i < n; i++) {
        a[i] *= b[i];
    }
    
    fft(a, true);
    
    a.resize(exact_size);

    while (a.size() > 1 && std::abs(a.back().real()) < 1e-9 && std::abs(a.back().imag()) < 1e-9) {
        a.pop_back();
    }
    
    return a;
}