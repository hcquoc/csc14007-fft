#include <iostream>
#include <vector>
#include <complex>
#include <cmath>
#include <chrono>
#include <random>
#include <iomanip>

using namespace std;
using cd = complex<double>;
const double PI = acos(-1);

// Dùng size_t thay cho int để vượt qua giới hạn 2.14 tỷ của số nguyên 32-bit
// ---------------------------------------------------------
// 1. THUẬT TOÁN NGÂY THƠ
// ---------------------------------------------------------
vector<double> multiply_naive(const vector<double>& A, const vector<double>& B) {
    size_t n = A.size();
    vector<double> res(2 * n - 1, 0.0);
    for (size_t i = 0; i < n; i++) {
        for (size_t j = 0; j < n; j++) {
            res[i + j] += A[i] * B[j];
        }
    }
    return res;
}

// ---------------------------------------------------------
// 2. THUẬT TOÁN KARATSUBA
// ---------------------------------------------------------
vector<double> multiply_karatsuba(const vector<double>& A, const vector<double>& B) {
    size_t n = A.size();
    vector<double> res(2 * n - 1, 0.0);
    
    if (n <= 64) return multiply_naive(A, B);
    
    size_t half = n / 2;
    vector<double> A0(A.begin(), A.begin() + half);
    vector<double> A1(A.begin() + half, A.end());
    vector<double> B0(B.begin(), B.begin() + half);
    vector<double> B1(B.begin() + half, B.end());

    vector<double> Z0 = multiply_karatsuba(A0, B0);
    vector<double> Z2 = multiply_karatsuba(A1, B1);

    vector<double> Asum(half), Bsum(half);
    for (size_t i = 0; i < half; i++) {
        Asum[i] = A0[i] + A1[i];
        Bsum[i] = B0[i] + B1[i];
    }
    vector<double> Z1 = multiply_karatsuba(Asum, Bsum);

    for (size_t i = 0; i < 2 * half - 1; i++) Z1[i] -= (Z0[i] + Z2[i]);

    for (size_t i = 0; i < 2 * half - 1; i++) {
        res[i] += Z0[i];
        res[i + half] += Z1[i];
        res[i + 2 * half] += Z2[i];
    }
    return res;
}

// ---------------------------------------------------------
// 3. THUẬT TOÁN FFT 
// ---------------------------------------------------------
void fft(vector<cd>& a, bool invert) {
    size_t n = a.size();
    for (size_t i = 1, j = 0; i < n; i++) {
        size_t bit = n >> 1;
        for (; j & bit; bit >>= 1) j ^= bit;
        j ^= bit;
        if (i < j) swap(a[i], a[j]);
    }
    for (size_t len = 2; len <= n; len <<= 1) {
        double ang = 2 * PI / len * (invert ? -1 : 1);
        cd wlen(cos(ang), sin(ang));
        for (size_t i = 0; i < n; i += len) {
            cd w(1);
            for (size_t j = 0; j < len / 2; j++) {
                cd u = a[i+j], v = a[i+j+len/2] * w;
                a[i+j] = u + v;
                a[i+j+len/2] = u - v;
                w *= wlen;
            }
        }
    }
    if (invert) for (cd & x : a) x /= n;
}

vector<double> multiply_fft(const vector<double>& A, const vector<double>& B) {
    size_t n = A.size();
    size_t out_size = 2 * n - 1;
    
    size_t pow2_size = 1;
    while (pow2_size < out_size) pow2_size <<= 1;
    
    vector<cd> fa(A.begin(), A.end()), fb(B.begin(), B.end());
    fa.resize(pow2_size); fb.resize(pow2_size);
    
    fft(fa, false);
    fft(fb, false);
    for (size_t i = 0; i < pow2_size; i++) fa[i] *= fb[i];
    fft(fa, true);
    
    vector<double> res(out_size);
    for (size_t i = 0; i < out_size; i++) res[i] = fa[i].real();
    return res;
}

// ---------------------------------------------------------
// HÀM CHẠY BENCHMARK
// ---------------------------------------------------------
int main() {
    mt19937 rng(42);
    uniform_real_distribution<double> dist(0.0, 100.0);

    cout << left << setw(15) << "Size (N)" 
         << setw(20) << "Naive (ms)" 
         << setw(20) << "Karatsuba (ms)" 
         << setw(20) << "FFT (ms)" << endl;
    cout << string(75, '-') << endl;

    // Chạy từ 2^8 (256) lên tối đa 2^24 (16.7 triệu). 
    // Nếu bạn thuê Cloud Server AWS/GCP có 512GB RAM, bạn có thể sửa số 24 thành 32!
    for (int k = 8; k <= 24; k++) {
        // Dùng 1ULL để bit-shift an toàn với số nguyên 64-bit
        size_t N = 1ULL << k; 
        vector<double> A(N), B(N);
        for (size_t i = 0; i < N; i++) {
            A[i] = dist(rng);
            B[i] = dist(rng);
        }

        // Đo Naive (Dừng ở N=16384)
        double time_naive = -1.0;
        if (N <= 16384) {
            auto start = chrono::high_resolution_clock::now();
            auto res_naive = multiply_naive(A, B);
            auto end = chrono::high_resolution_clock::now();
            time_naive = chrono::duration<double, milli>(end - start).count();
        }

        // Đo Karatsuba (Dừng ở N=131072)
        double time_karatsuba = -1.0;
        if (N <= 131072) {
            auto start_k = chrono::high_resolution_clock::now();
            auto res_k = multiply_karatsuba(A, B);
            auto end_k = chrono::high_resolution_clock::now();
            time_karatsuba = chrono::duration<double, milli>(end_k - start_k).count();
        }

        // Đo FFT 
        auto start_f = chrono::high_resolution_clock::now();
        auto res_f = multiply_fft(A, B);
        auto end_f = chrono::high_resolution_clock::now();
        double time_fft = chrono::duration<double, milli>(end_f - start_f).count();

        // In kết quả
        cout << left << setw(15) << N;
        if (time_naive < 0) cout << setw(20) << "D/N Finish";
        else cout << setw(20) << fixed << setprecision(3) << time_naive;
        
        if (time_karatsuba < 0) cout << setw(20) << "D/N Finish";
        else cout << setw(20) << time_karatsuba;
        
        cout << setw(20) << time_fft << endl;
    }

    return 0;
}