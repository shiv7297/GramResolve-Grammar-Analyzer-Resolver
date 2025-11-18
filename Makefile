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
REPORT_DIR = src/report
REPORT_FILE = $(REPORT_DIR)/report.txt

# Find all .cpp files recursively
SRCS = $(shell find $(SRC_DIR) -name '*.cpp')
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

# Default target
all: prepare_report $(BIN)

# Prepare report file (clear it)
prepare_report:
	@mkdir -p $(REPORT_DIR)
	@echo "📝 Clearing old report..."
	@> $(REPORT_FILE)

# Link all object files into the final executable
$(BIN): $(OBJS)
	@echo "🔗 Linking object files..."
	$(CXX) $(OBJS) -o $(BIN)
	@echo "✅ Build complete: $(BIN)"

# Compile each .cpp into .o file inside build folder
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@echo "🧱 Compiling $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run program after building
run: all
	@echo "🚀 Running program..."
	./$(BIN)

# Explicit target to only regenerate report without rebuilding
report:
	@mkdir -p $(REPORT_DIR)
	@echo "📝 Resetting report..."
	@> $(REPORT_FILE)

# Clean build and executable
clean:
	@echo "🧹 Cleaning build files..."
	rm -rf $(BUILD_DIR) $(BIN) $(REPORT_FILE)

.PHONY: all run clean report prepare_report
