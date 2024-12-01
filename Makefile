# Compiler
CXX = g++
# Compiler flags
CXXFLAGS = -O2 -std=c++17
# Linker flags
LDFLAGS = 

# Project files
TARGET = build/main
SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:%.cpp=build/%.o)

# Create the build directory if it doesn't exist
BUILD_DIR = build
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Build the executable
$(TARGET): $(BUILD_DIR) $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^

# Compile source files to object files
build/%.o: %.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Run the program
run: $(TARGET)
	./$(TARGET)

# Clean up build files
clean:
	rm -rf $(BUILD_DIR)

# For debugging, build with debug flags
debug: CXXFLAGS += -g
debug: clean $(TARGET)

.PHONY: clean debug run
