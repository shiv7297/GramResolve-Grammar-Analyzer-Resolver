# ================================
# Makefile for Grammar Analyzer
# ================================

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Iinclude -Isrc -Wall -Wextra

# Directories
SRC_DIR = src
BUILD_DIR = build
BIN = text.exe

# Source files (all .cpp files under src/)
SRCS = $(wildcard $(SRC_DIR)/**/*.cpp) $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

# Default target
all: $(BIN)

# Link all object files into the final executable
$(BIN): $(OBJS)
	@echo "🔗 Linking object files..."
	$(CXX) $(OBJS) -o $(BIN)
	@echo "✅ Build complete: $(BIN)"

# Compile each .cpp to .o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@echo "🧱 Compiling $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run the program
run: $(BIN)
	@echo "🚀 Running program..."
	./$(BIN)

# Clean up build artifacts
clean:
	@echo "🧹 Cleaning build files..."
	rm -rf $(BUILD_DIR) $(BIN)

# Phony targets (not actual files)
.PHONY: all run clean
