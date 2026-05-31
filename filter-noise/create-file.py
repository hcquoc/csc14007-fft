import librosa
import soundfile as sf
import numpy as np

# 1. Cấu hình file và tham số
input_file = 'the-weekend-star-boy.mp3'
output_file = 'the-weekend-star-boy-noisy.wav'

# Chọn tần số nhiễu: 3000Hz (tiếng rít chói tai giống tiếng muỗi kêu lớn)
noise_freq = 3000 
# Cường độ nhiễu (0.1 đến 0.2 là đủ để nghe rõ mà không làm điếc tai)
noise_intensity = 0.15 

print("Đang đọc file âm thanh (có thể mất vài giây)...")
# 2. Đọc file MP3 (sr=None để giữ nguyên tần số lấy mẫu gốc, thường là 44100Hz)
audio_data, sample_rate = librosa.load(input_file, sr=None, mono=True)

# Lấy thời gian của từng điểm dữ liệu (t = mảng thời gian)
total_samples = len(audio_data)
t = np.arange(total_samples) / sample_rate

print(f"Đã tải xong! Tần số lấy mẫu: {sample_rate} Hz, Tổng số mẫu: {total_samples}")
print(f"Tạo tiếng nhiễu ở tần số {noise_freq}Hz...")


noise = noise_intensity * np.max(audio_data) * np.sin(2 * np.pi * noise_freq * t)

# 4. Trộn nhiễu vào bài hát
noisy_audio = audio_data + noise

# Cắt cúp (clip) dữ liệu để đảm bảo không bị vượt quá giới hạn âm thanh (tránh rách loa)
noisy_audio = np.clip(noisy_audio, -1.0, 1.0)

print("Đang lưu file nhiễu...")
# 5. Lưu lại thành file WAV để test cho các bước sau
sf.write(output_file, noisy_audio, sample_rate)

print(f"Hoàn tất! File đã bị làm nhiễu được lưu tại: {output_file}")
