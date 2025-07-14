# Compiler and flags
CXX = g++
TIDY = clang-tidy
CXXFLAGS = -O3 -Wall -std=c++20 -I/usr/include -I/usr/local/include -I/usr/include/libmongoc-1.0 -I/usr/include/libbson-1.0 -L/usr/lib/x86_64-linux-gnu -march=native -pthread
LDFLAGS = -static-libgcc -static-libstdc++ -luuid -lcjson -lcurl -lbson-1.0 -lmongoc-1.0 -pthread

# Strategies and decks
STRATEGIES := mimic linear polynomial neural basic high-low wong
DECKS := single-deck double-deck six-shoe

# .PHONY targets
.PHONY: all clean lint bear install help

# Directories
SRC_DIR = src
SRC_DIRS = arguments cards constants table simulator xlog
INCLUDE_DIRS = $(SRC_DIRS)
OBJ_DIR = obj

# Includes
INCLUDES = $(foreach dir,$(SRC_DIRS), -I$(SRC_DIR)/$(dir))

# Files
SRC_FILES = $(wildcard $(SRC_DIR)/main.cpp $(foreach dir,$(SRC_DIRS),$(SRC_DIR)/$(dir)/*.cpp))
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

# Binary
TARGET = bin/striker-plus
STRIKER = ${HOME}/Striker

# Default
.DEFAULT_GOAL := help

help:
	@echo "Makefile for Striker-plus project"
	@echo "  all            - Build the binary"
	@echo "  clean          - Remove build artifacts"
	@echo "  lint           - Run clang-tidy"
	@echo "  bear           - Generate compile_commands.json"
	@echo "  install        - Install binary to $(STRIKER)/bin"

# Build
all: $(TARGET)

$(TARGET): $(OBJ_FILES)
	@mkdir -p bin
	$(CXX) $(CXXFLAGS) $(INCLUDES) -o $@ $^ $(LDFLAGS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(TARGET)

lint:
	$(TIDY) $(SRC_FILES)

bear:
	bear -- make clean all

install:
	cp -rf $(TARGET) $(STRIKER)/bin

