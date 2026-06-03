#pragma once
#include <vector>
#include <complex>
#include "fft.hpp"

std::vector<cd> multiply_polynomial(const std::vector<cd>& a, const std::vector<cd>& b);

std::vector<cd> chirp_z(std::vector<cd>& a);