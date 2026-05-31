import numpy as np
import soundfile as sf
import matplotlib.pyplot as plt

# 1. Đọc 3 file âm thanh
orig_audio, sr = sf.read('the-weekend-star-boy.mp3') 
noisy_audio, _ = sf.read('the-weekend-star-boy-noisy.wav')
clean_audio, _ = sf.read('the-weekend-star-boy-clean.wav')

# Tự động gộp 2 kênh thành 1 kênh (Stereo -> Mono) cho file gốc nếu có
if len(orig_audio.shape) > 1:
    orig_audio = np.mean(orig_audio, axis=1)

# Để code chạy nhanh hơn và biểu đồ dễ nhìn, ta chỉ lấy 5 giây đầu tiên của bài hát
N_samples = 5 * sr 
orig_audio = orig_audio[:N_samples]
noisy_audio = noisy_audio[:N_samples]
clean_audio = clean_audio[:N_samples]

# -----------------------------------------------------
# BẰNG CHỨNG TOÁN HỌC (Tính sai số MSE)
# -----------------------------------------------------
mse = np.mean((orig_audio - clean_audio)**2)
print(f"Chỉ số sai số MSE giữa bản gốc và bản đã lọc: {mse:.8f}")

# ... (Giữ nguyên phần vẽ biểu đồ bên dưới) ...
fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(10, 8))

# Vẽ phổ của file bị nhiễu
Pxx, freqs, bins, im1 = ax1.specgram(noisy_audio, NFFT=2048, Fs=sr, cmap='magma')
ax1.set_title("Spectrogram: Before Filtering (Notice the horizontal line at 3000Hz)")
ax1.set_ylabel("Frequency (Hz)")
# Giới hạn trục y ở 5000Hz để nhìn rõ vạch nhiễu 3000Hz
ax1.set_ylim(0, 5000) 
fig.colorbar(im1, ax=ax1, label='Intensity (dB)')

# Vẽ phổ của file đã lọc
Pxx, freqs, bins, im2 = ax2.specgram(clean_audio, NFFT=2048, Fs=sr, cmap='magma')
ax2.set_title("Spectrogram: After FFT Filter (The 3000Hz noise is completely removed)")
ax2.set_xlabel("Time (seconds)")
ax2.set_ylabel("Frequency (Hz)")
ax2.set_ylim(0, 5000)
fig.colorbar(im2, ax=ax2, label='Intensity (dB)')

plt.tight_layout()
plt.savefig('bieu_do_ket_qua.png', dpi=300)