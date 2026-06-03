CXX = g++
CXXFLAGS = -std=c++17 -O3 -Iinclude

SRC_DIR = src
BENCH_DIR = benchmark
BUILD_DIR = build
TARGET = $(BUILD_DIR)/benchmark_runner

SRCS = $(SRC_DIR)/fft.cpp $(SRC_DIR)/ntt.cpp $(SRC_DIR)/chirp_z.cpp $(SRC_DIR)/baselines.cpp $(BENCH_DIR)/benchmark.cpp

all: $(TARGET)

$(TARGET): $(SRCS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET)
	@echo "===================================================="
	@echo " Compilation successful! Type 'make run' to execute."
	@echo "===================================================="

run: $(TARGET)
	@./$(TARGET)

clean:
	rm -rf $(BUILD_DIR)
	@echo "Cleaned up build artifacts."

.PHONY: all run clean