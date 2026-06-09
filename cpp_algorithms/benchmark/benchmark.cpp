#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <iomanip>
#include <complex>

#include "common.hpp"
#include "fft.hpp"
#include "chirp_z.hpp"
#include "baselines.hpp"

using namespace std;
using namespace std::chrono;

mt19937 rng(42); 

// ==============================================================================
// HÀM SINH DỮ LIỆU ĐA THỨC HỆ SỐ THỰC
// ==============================================================================
vector<cd> generate_random_real(int n) {
    uniform_real_distribution<double> dist(-10.0, 10.0);
    vector<cd> a(n);
    for (int i = 0; i < n; i++) {
        a[i] = cd(dist(rng), 0.0);
    }
    return a;
}

int main() {
    cout << fixed << setprecision(3);

    cout << "\n=========================================================================================\n";
    cout << "              REAL POLYNOMIAL MULTIPLICATION PERFORMANCE BENCHMARK                       \n";
    cout << "=========================================================================================\n\n";

    // *****************************************************************************************
    // BẢNG 1: KÍCH THƯỚC LŨY THỪA CỦA 2
    // *****************************************************************************************
    cout << "--- TABLE 1: POWER-OF-TWO SIZES ---\n";
    cout << left << setw(10) << "Size (N)" 
         << left << setw(15) << "Naive (ms)" 
         << left << setw(18) << "Karatsuba (ms)" 
         << left << setw(16) << "Chirp-Z (ms)"
         << left << setw(16) << "FFT Mult (ms)" << "\n";
    cout << "-----------------------------------------------------------------------------------------\n";

    vector<int> pow2_sizes = {1024, 4096, 16384, 65536, 262144, 524288, 1048576};
    for (int n : pow2_sizes) {
        cout << left << setw(10) << n;
        
        // 1. Naive 
        if (n <= 262144) {
            auto a = generate_random_real(n); vector<cd> b = generate_random_real(n);
            auto start = high_resolution_clock::now();
            auto res = naive_multiply(a, b);
            cout << left << setw(15) << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1000.0;
        } else {
            cout << left << setw(15) << "Skip (>262k)";
        }

        // 2. Karatsuba
        // if (n <= 524288) {
        auto a = generate_random_real(n); vector<cd> b = generate_random_real(n);
        auto start = high_resolution_clock::now();
        auto res = karatsuba_multiply(a, b);
        cout << left << setw(18) << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1000.0;
        // } else {
        //     cout << left << setw(18) << "Skip (>524k)";
        // }

        // 3. Chirp-Z
        auto data_czt = generate_random_real(n);
        auto start_czt = high_resolution_clock::now();
        auto res_czt = chirp_z(data_czt);
        cout << left << setw(16) << duration_cast<microseconds>(high_resolution_clock::now() - start_czt).count() / 1000.0;

        // 4. FFT Multiply
        auto a_fft = generate_random_real(n); vector<cd> b_fft = generate_random_real(n);
        auto start_fft = high_resolution_clock::now();
        auto res_fft = multiply_fft(a_fft, b_fft);
        cout << left << setw(16) << duration_cast<microseconds>(high_resolution_clock::now() - start_fft).count() / 1000.0 << "\n";
    }

    // *****************************************************************************************
    // BẢNG 2: KÍCH THƯỚC KHÔNG PHẢI LŨY THỪA CỦA 2
    // *****************************************************************************************
    cout << "\n--- TABLE 2: NON-POWER-OF-TWO SIZES ---\n";
    cout << left << setw(10) << "Size (N)" 
         << left << setw(15) << "Naive (ms)" 
         << left << setw(18) << "Karatsuba (ms)" 
         << left << setw(16) << "Chirp-Z (ms)"
         << left << setw(16) << "FFT Mult (ms)" << "\n";
    cout << "-----------------------------------------------------------------------------------------\n";

    vector<int> non_pow2_sizes = {1500, 5000, 12000, 55000, 200000, 500000, 1000000};
    for (int n : non_pow2_sizes) {
        cout << left << setw(10) << n;

        // 1. Naive
        if (n <= 200000) {
            auto a = generate_random_real(n); vector<cd> b = generate_random_real(n);
            auto start = high_resolution_clock::now();
            auto res = naive_multiply(a, b);
            cout << left << setw(15) << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1000.0;
        } else {
            cout << left << setw(15) << "Skip (>200k)";
        }

        // 2. Karatsuba
        // if (n <= 5000000) {
        auto a = generate_random_real(n); vector<cd> b = generate_random_real(n);
        auto start = high_resolution_clock::now();
        auto res = karatsuba_multiply(a, b);
        cout << left << setw(18) << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1000.0;
        // } else {
        //     cout << left << setw(18) << "Skip (>500k)";
        // }

        // 3. Chirp-Z
        auto data_czt = generate_random_real(n);
        auto start_czt = high_resolution_clock::now();
        auto res_czt = chirp_z(data_czt);
        cout << left << setw(16) << duration_cast<microseconds>(high_resolution_clock::now() - start_czt).count() / 1000.0;

        // 4. FFT Multiply
        auto a_fft = generate_random_real(n); vector<cd> b_fft = generate_random_real(n);
        auto start_fft = high_resolution_clock::now();
        auto res_fft = multiply_fft(a_fft, b_fft);
        cout << left << setw(16) << duration_cast<microseconds>(high_resolution_clock::now() - start_fft).count() / 1000.0 << "\n";
    }

    cout << "=========================================================================================\n";

    return 0;
}