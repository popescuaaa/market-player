# Compiler and flags
CXX := g++
CXXFLAGS := -std=c++17 -g

# Directories
SRC_DIR := src
BUILD_DIR := build

# Find all source files in the src/ directory
SRCS := $(wildcard $(SRC_DIR)/*.cpp)
# Convert source file paths to object file paths in build/
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(BUILD_DIR)/%.o, $(SRCS))

# Name of the final executable
TARGET := $(BUILD_DIR)/app

# Default target
all: $(TARGET)

# Rule to build the executable
$(TARGET): $(OBJS)
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Rule to build object files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to run the program
run: all
	./$(TARGET)

# Clean up build artifacts
clean:
	rm -rf $(BUILD_DIR)

# Phony targets
.PHONY: all run clean
