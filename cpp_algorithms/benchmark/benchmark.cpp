#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <iomanip>
#include <complex>

#include "common.hpp"
#include "fft.hpp"
#include "ntt.hpp"
#include "chirp_z.hpp"
#include "baselines.hpp"

using namespace std;
using namespace std::chrono;

mt19937 rng(42); 

vector<cd> generate_random_complex(int n) {
    uniform_real_distribution<double> dist(-10.0, 10.0);
    vector<cd> a(n);
    for (int i = 0; i < n; i++) a[i] = cd(dist(rng), dist(rng));
    return a;
}

vector<long long> generate_random_integer(int n) {
    uniform_int_distribution<long long> dist(0, MOD - 1);
    vector<long long> a(n);
    for (int i = 0; i < n; i++) a[i] = dist(rng);
    return a;
}

int main() {
    cout << fixed << setprecision(3);

    // =========================================================================================
    // BẢNG 1: KÍCH THƯỚC LŨY THỪA CỦA 2 (SO SÁNH ĐẦY ĐỦ 5 THUẬT TOÁN)
    // =========================================================================================
    cout << "=========================================================================================\n";
    cout << "             TABLE 1: PERFORMANCE MATRIX FOR POWER-OF-TWO SIZES (5 ALGORITHMS)           \n";
    cout << "=========================================================================================\n";
    cout << left << setw(10) << "Size (N)" 
         << left << setw(15) << "Naive (ms)" 
         << left << setw(18) << "Karatsuba (ms)" 
         << left << setw(16) << "Chirp-Z (ms)"
         << left << setw(14) << "FFT (ms)"
         << left << setw(14) << "NTT (ms)" << "\n";
    cout << "-----------------------------------------------------------------------------------------\n";

    vector<int> pow2_sizes = {1024, 4096, 16384, 65536, 262144};

    for (int n : pow2_sizes) {
        cout << left << setw(10) << n;

        // 1. Naive
        if (n <= 65536) {
            auto a = generate_random_complex(n); vector<cd> b = generate_random_complex(n);
            auto start = high_resolution_clock::now();
            auto res = naive_multiply(a, b);
            cout << left << setw(15) << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1000.0;
        } else {
            cout << left << setw(15) << "Skip (>65k)";
        }

        // 2. Karatsuba
        if (n <= 262144) {
            auto a = generate_random_complex(n); vector<cd> b = generate_random_complex(n);
            auto start = high_resolution_clock::now();
            auto res = karatsuba_multiply(a, b);
            cout << left << setw(18) << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1000.0;
        } else {
            cout << left << setw(18) << "Skip (>65k)";
        }

        // 3. Chirp-Z
        auto data_czt = generate_random_complex(n);
        auto start_czt = high_resolution_clock::now();
        auto res_czt = chirp_z(data_czt);
        cout << left << setw(16) << duration_cast<microseconds>(high_resolution_clock::now() - start_czt).count() / 1000.0;

        // 4. FFT
        auto data_fft = generate_random_complex(n);
        auto start_fft = high_resolution_clock::now();
        fft(data_fft, false);
        cout << left << setw(14) << duration_cast<microseconds>(high_resolution_clock::now() - start_fft).count() / 1000.0;

        // 5. NTT
        auto data_ntt = generate_random_integer(n);
        auto start_ntt = high_resolution_clock::now();
        ntt(data_ntt, false);
        cout << left << setw(14) << duration_cast<microseconds>(high_resolution_clock::now() - start_ntt).count() / 1000.0 << "\n";
    }
    cout << "=========================================================================================\n\n";


    // =========================================================================================
    // BẢNG 2: KÍCH THƯỚC KHÔNG PHẢI LŨY THỪA CỦA 2 (SO SÁNH 3 THUẬT TOÁN)
    // =========================================================================================
    cout << "=====================================================================\n";
    cout << "       TABLE 2: PERFORMANCE MATRIX FOR NON-POWER-OF-TWO SIZES        \n";
    cout << "=====================================================================\n";
    cout << left << setw(12) << "Size (N)" 
         << left << setw(15) << "Naive (ms)" 
         << left << setw(18) << "Karatsuba (ms)" 
         << left << setw(16) << "Chirp-Z (ms)" << "\n";
    cout << "---------------------------------------------------------------------\n";

    vector<int> non_pow2_sizes = {1500, 5000, 12000, 55000, 200000};

    for (int n : non_pow2_sizes) {
        cout << left << setw(12) << n;

        // 1. Naive
        if (n <= 55000) {
            auto a = generate_random_complex(n); vector<cd> b = generate_random_complex(n);
            auto start = high_resolution_clock::now();
            auto res = naive_multiply(a, b);
            cout << left << setw(15) << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1000.0;
        } else {
            cout << left << setw(15) << "Skip (>55k)";
        }

        // 2. Karatsuba
        if (n <= 200000) {
            auto a = generate_random_complex(n); vector<cd> b = generate_random_complex(n);
            auto start = high_resolution_clock::now();
            auto res = karatsuba_multiply(a, b);
            cout << left << setw(18) << duration_cast<microseconds>(high_resolution_clock::now() - start).count() / 1000.0;
        } else {
            cout << left << setw(18) << "Skip (>200k)";
        }

        // 3. Chirp-Z
        auto data_czt = generate_random_complex(n);
        auto start_czt = high_resolution_clock::now();
        auto res_czt = chirp_z(data_czt);
        cout << left << setw(16) << duration_cast<microseconds>(high_resolution_clock::now() - start_czt).count() / 1000.0 << "\n";
    }
    cout << "=====================================================================\n";

    return 0;
}