# Compiler and flags
CXX = g++
CXXFLAGS = -O3 -Wall -std=c++20 -I/usr/include -I/usr/local/include

# Directories
SRC_DIR = src
SRC_DIRS = arguments cards constants table simulator machine
INCLUDE_DIRS = $(SRC_DIRS)
OBJ_DIR = obj

# Include all directories for header files
INCLUDES = $(foreach dir, $(INCLUDE_DIRS), -I$(SRC_DIR)/$(dir)) -I/usr/local/Cellar/eigen/3.4.0_1/include/eigen3

# Source files (located in the src/ directory)
SRC_FILES = $(wildcard $(SRC_DIR)/main.cpp $(foreach dir, $(SRC_DIRS), $(SRC_DIR)/$(dir)/*.cpp))

# Object files (place them in the obj/ directory)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC_FILES))

# Output binary
TARGET = bin/strikerC++

# Default target
all: $(TARGET)

# Build target
$(TARGET): $(OBJ_FILES)
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $(TARGET) $(OBJ_FILES) -luuid -lcjson -lcurl

# Compile source files into object files in obj/ directory
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean up object files and the binary
clean:
	rm -f $(OBJ_DIR)/*.o $(OBJ_DIR)/*/*.o $(TARGET)

