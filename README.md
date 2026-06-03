# FFT and Applications

A comprehensive framework focused on the Fast Fourier Transform (FFT), Number Theoretic Transform (NTT), and Chirp-Z Transform (CZT) optimized for performance using C++ (Core Engine), alongside practical 1D/2D Digital Signal Processing (DSP) applications implemented in Python.

## 📌 Project Overview

This project was developed to analyze algorithmic complexity and evaluate the real-world performance of polynomial multiplication and frequency transformation algorithms. The system is split into two primary subsystems:
1. **Core Engine (C++17):** Implements an iterative, bottom-up, in-place (non-recursive) architecture optimized for Cache Locality covering FFT, NTT, and CZT. It provides direct performance comparisons against baseline Naive $O(N^2)$ and Karatsuba $O(N^{1.58})$ algorithms.
2. **Applications (Python/Jupyter):** Applies the power of the Fourier domain to practical signal processing tasks, including 1D Audio Denoising (removing whistle tones and high-frequency hiss) and 2D Image Periodic Noise Removal (Notch filtering).

---

## 📂 Directory Structure

```text
project/
├── cpp_algorithms/                  # C++ Core Engine (Performance Benchmarking)
│   ├── include/                     # Header Files (.hpp)
│   │   ├── common.hpp               # High-precision PI, Generic Bit-Reversal Template
│   │   ├── fft.hpp
│   │   ├── ntt.hpp
│   │   ├── chirp_z.hpp
│   │   └── baselines.hpp            # Naive & Karatsuba Multiplications
│   ├── src/                         # Implementation Files (.cpp)
│   │   ├── fft.cpp
│   │   ├── ntt.cpp
│   │   ├── chirp_z.cpp
│   │   └── baselines.cpp
│   ├── benchmark/
│   │   └── benchmark.cpp            # Performance Matrix Test Bench
│   └── Makefile                     # Automation script with high optimization flags (-O3)
├── python_applications/             # Python Applications (Jupyter Notebooks)
│   ├── audio_denoising.ipynb        # Single-tone whistle & broadband hiss removal
│   └── image_processing.ipynb       # Periodic stripe noise removal on Lena image
└── data/                            # Experimental Datasets
    ├── audio/                       # Audio files (.mp3, .wav for Original, Noisy, Clean)
    └── images/                      # Standard test images (lena.png)