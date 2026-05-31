import matplotlib.pyplot as plt
import numpy as np

# Dữ liệu từ file C++ benchmark
N = [256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536]
time_naive = [0.875, 3.790, 20.342, 64.150, 232.759, 906.276, 3566.588, None, None]
time_karatsuba = [0.568, 1.704, 8.007, 17.015, 47.325, 144.305, 423.384, 1295.840, 3843.499]
time_fft = [0.544, 1.156, 2.541, 5.553, 11.706, 25.389, 53.626, 114.946, 243.093]

plt.figure(figsize=(10, 6))

# Vẽ 3 đường đồ thị với các marker khác nhau
plt.plot(N, time_naive, marker='o', linestyle='-', color='red', label='Naive O(N^2)')
plt.plot(N, time_karatsuba, marker='s', linestyle='--', color='orange', label='Karatsuba O(N^1.585)')
plt.plot(N, time_fft, marker='^', linestyle='-.', color='green', label='FFT O(N log N)')

# Thiết lập trục
plt.xlabel('Polynomial Size (N)', fontsize=12)
plt.ylabel('Execution Time (ms)', fontsize=12)
plt.title('Benchmark: Polynomial Multiplication Algorithms', fontsize=14, fontweight='bold')

# Sử dụng thang đo Logarit cho cả 2 trục để thấy rõ sự khác biệt về độ phức tạp
plt.xscale('log', base=2)
plt.yscale('log')

plt.grid(True, which="both", ls="--", alpha=0.5)
plt.legend(fontsize=12)
plt.tight_layout()

# Lưu ảnh chất lượng cao để chèn vào slide
plt.savefig('benchmark_chart_log.png', dpi=300)
print("Đã lưu biểu đồ thành công!")