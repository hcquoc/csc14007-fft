import numpy as np
import soundfile as sf
import time

# File configuration
input_file = 'the-weekend-star-boy-noisy.wav'
output_file = 'the-weekend-star-boy-clean.wav'

print("Loading noisy audio into memory...")
# --- MEASURE FILE READ TIME ---
read_start = time.time()
audio_data, sample_rate = sf.read(input_file)
read_end = time.time()
# ------------------------------

N = len(audio_data)

print(f"Total polynomial coefficients (Data points): {N:,}")
print(f"Disk read time: {read_end - read_start:.4f} seconds")
print("-" * 50)

# Start total processing timer (excluding file I/O)
process_start_time = time.time()

# ---------------------------------------------------------
# STEP 1: Transform audio to frequency domain -> Equivalent to DFT(S)
# ---------------------------------------------------------
print("1. Running FFT to transform audio polynomial S(x)...")
# --- MEASURE FORWARD FFT TIME ---
fft_start = time.time()
S_freq = np.fft.fft(audio_data)
fft_end = time.time()
print(f"FFT execution time: {fft_end - fft_start:.4f} seconds")
# --------------------------------

# Extract frequency axis (Hz) corresponding to each coefficient
frequencies = np.fft.fftfreq(N, 1/sample_rate)

# ---------------------------------------------------------
# STEP 2: Design static filter -> Equivalent to DFT(F)
# ---------------------------------------------------------
print("2. Creating filter F(x) to isolate the 3000Hz band...")
# Initialize filter with an array of ones (1 = allow signal to pass)
F_freq = np.ones(N)

# Locate the 3000Hz high-frequency noise (allow +- 10Hz margin)
noise_indices = (np.abs(frequencies) > 2990) & (np.abs(frequencies) < 3010)

# Set coefficients at noise frequencies to 0 (0 = block signal, cancel noise)
F_freq[noise_indices] = 0.0

# ---------------------------------------------------------
# STEP 3: Multiply the two polynomials
# ---------------------------------------------------------
print("3. Multiplying polynomials C = DFT(S) * DFT(F)...")
C_freq = S_freq * F_freq

# ---------------------------------------------------------
# STEP 4: Inverse transform back to time domain -> Equivalent to Inverse FFT
# ---------------------------------------------------------
print("4. Running Inverse FFT to recover original audio...")
# --- MEASURE INVERSE FFT TIME ---
ifft_start = time.time()
clean_audio = np.fft.ifft(C_freq).real
ifft_end = time.time()
print(f"IFFT execution time: {ifft_end - ifft_start:.4f} seconds")
# --------------------------------

# End total processing timer
process_end_time = time.time()
# ---------------------------------------------------------

print("-" * 50)
print(f"TOTAL ALGORITHM PROCESSING TIME (excluding file I/O): {process_end_time - process_start_time:.4f} seconds!")

print("\nExporting clean audio file...")
write_start = time.time()
sf.write(output_file, clean_audio, sample_rate)
write_end = time.time()
print(f"Disk write time: {write_end - write_start:.4f} seconds")

print(f"\nSuccess! Cleaned file saved at: {output_file}")