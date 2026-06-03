#pragma once
#include <vector>
#include "fft.hpp" 

std::vector<cd> naive_multiply(const std::vector<cd>& a, const std::vector<cd>& b);

std::vector<cd> karatsuba_multiply(std::vector<cd> a, std::vector<cd> b);