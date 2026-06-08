#pragma once
#include <vector>
#include <complex>

using cd = std::complex<double>;

void fft(std::vector<cd>& a, bool invert);

std::vector<cd> multiply_fft(std::vector<cd> a, std::vector<cd> b);