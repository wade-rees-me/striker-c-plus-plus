# Compiler and flags
CXX = g++
TIDY = clang-tidy
CXXFLAGS = -O3 -Wall -std=c++20 -I/usr/include -I/usr/local/include -I/usr/include/libmongoc-1.0 -I/usr/include/libbson-1.0 -L/usr/lib/x86_64-linux-gnu -march=native -pthread
LDFLAGS = -static-libgcc -static-libstdc++ -luuid -lcjson -lcurl -lbson-1.0 -lmongoc-1.0 -pthread

# Strategies and decks
STRATEGIES := mimic linear polynomial neural basic high-low wong
DECKS := single-deck double-deck six-shoe

# .PHONY targets
.PHONY: all clean lint bear install help run run-all \
	$(foreach s,$(STRATEGIES),run-$(s) $(foreach d,$(DECKS),run-$(s)-$(d))) \
	run-single-deck run-double-deck run-six-shoe \
	rm rm1 rm2 rm6 rl rl1 rl2 rl6 rp rp1 rp2 rp6 rn rn1 rn2 rn6 \
	rb rb1 rb2 rb6 rh rh1 rh2 rh6 rw rw1 rw2 rw6

# Directories
SRC_DIR = src
SRC_DIRS = arguments cards constants table simulator aws
INCLUDE_DIRS = $(SRC_DIRS)
OBJ_DIR = obj

# Includes
INCLUDES = $(foreach dir,$(SRC_DIRS), -I$(SRC_DIR)/$(dir))

# Files
SRC_FILES = $(wildcard $(SRC_DIR)/main.cpp $(foreach dir,$(SRC_DIRS),$(SRC_DIR)/$(dir)/*.cpp))
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp,$(OBJ_DIR)/%.o,$(SRC_FILES))

# Binary
TARGET = bin/strikerC++
STRIKER = ${HOME}/Striker

# Params
HANDS ?= 500000000
THREADS ?= 24
STRATEGY ?= mimic
DECKS ?= single-deck
LOG_DIR = $(STRIKER)/Simulations/$(shell date +%Y)/$(shell date +%m)/$(shell date +%d)
LOG = $(LOG_DIR)/$(notdir $(TARGET))-$(shell date +%H%M%S).log

# Default
.DEFAULT_GOAL := help

help:
	@echo "Makefile for StrikerC++ project"
	@echo "  all            - Build the binary"
	@echo "  clean          - Remove build artifacts"
	@echo "  lint           - Run clang-tidy"
	@echo "  bear           - Generate compile_commands.json"
	@echo "  install        - Install binary to $(STRIKER)/bin"
	@echo "  run            - Run a simulation"
	@echo "  run-all        - Run all strategy/deck combinations"

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

include Makefile.run

