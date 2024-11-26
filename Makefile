# Compiler
CXX = g++
# Compiler flags
CXXFLAGS = -O2 -std=c++17
# Linker flags
LDFLAGS = 

# Project files
TARGET = main
SRCS = $(wildcard *.cpp)
OBJS = $(SRCS:.cpp=.o)

# Build the executable
$(TARGET): $(OBJS)
	$(CXX) $(LDFLAGS) -o $@ $^

# Compile source files to object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJS) $(TARGET)

# For debugging, build with debug flags
debug: CXXFLAGS += -g
debug: clean $(TARGET)

.PHONY: clean debug
