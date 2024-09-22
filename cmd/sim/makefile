# Compiler and flags
CXX = g++
CXXFLAGS = -g -Wall -std=c++20 -I/usr/include -I/usr/local/include

# Directories
SRC_DIRS = arguments cards constants rules simulator utilities
INCLUDE_DIRS = $(SRC_DIRS)

# Include all directories for header files
INCLUDES = $(foreach dir, $(INCLUDE_DIRS), -I$(dir))

# Source files
SRC_FILES = $(wildcard main.cpp $(foreach dir, $(SRC_DIRS), $(dir)/*.cpp))

# Object files
OBJ_FILES = $(SRC_FILES:.cpp=.o)

# Output binary
TARGET = bin/strikerC++

# Default target
all: $(TARGET)

# Build target
$(TARGET): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(TARGET) $(OBJ_FILES) -luuid -lcjson -lcurl

# Compile source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean up object files and the binary
clean:
	rm -f $(OBJ_FILES) $(TARGET)

