#include "chirp_z.hpp"
#include "common.hpp" 
#include <cmath>

std::vector<cd> multiply_polynomials(const std::vector<cd>& a, const std::vector<cd>& b) {
    std::vector<cd> fa(a.begin(), a.end());
    std::vector<cd> fb(b.begin(), b.end());
    
    int n = 1;
    while (n < a.size() + b.size()) {
        n <<= 1;
    }
    fa.resize(n); 
    fb.resize(n);

    fft(fa, false);
    fft(fb, false);
    
    for (int i = 0; i < n; i++) {
        fa[i] *= fb[i];
    }
    
    fft(fa, true);
    return fa;
}

std::vector<cd> chirp_z(std::vector<cd>& a) {
    int n = a.size();
    std::vector<cd> result(n);
    
    std::vector<cd> W(2 * n);
    for (int i = -n + 1; i < n; i++) {
        double angle = PI * i * i / n; 
        W[i + n - 1] = cd(std::cos(angle), std::sin(angle));
    }

    std::vector<cd> polyA(n);
    std::vector<cd> polyB(2 * n - 1);
    
    for (int i = 0; i < n; i++) {
        polyA[i] = a[i] * cd(W[i + n - 1].real(), -W[i + n - 1].imag()); 
    }
    for (int i = 0; i < 2 * n - 1; i++) {
        polyB[i] = W[i];
    }

    std::vector<cd> conv = multiply_polynomials(polyA, polyB);

    for (int i = 0; i < n; i++) {
        result[i] = conv[i + n - 1] * cd(W[i + n - 1].real(), -W[i + n - 1].imag());
    }
    return result;
}