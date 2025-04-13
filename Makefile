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

# Run template
define run_template
run-$(1)-$(2):
	@mkdir -p $(LOG_DIR)
	@echo "Running: $(1) with $(2), $(HANDS) hands on $(THREADS) threads"
	clear
	./$(TARGET) --$(1) --$(2) --number-of-hands $(HANDS) --number-of-threads $(THREADS) | tee $(LOG)
	sleep 3
endef

$(foreach strategy,$(STRATEGIES), \
	$(foreach deck,$(DECKS), \
		$(eval $(call run_template,$(strategy),$(deck))) \
	) \
)

define group_template
run-$(1):
	$(foreach deck,$(DECKS), \
		$(MAKE) run-$(1)-$(deck); \
	)
endef

$(foreach strategy,$(STRATEGIES), \
	$(eval $(call group_template,$(strategy))) \
)

run-all:
	$(foreach strategy,$(STRATEGIES), \
		$(MAKE) run-$(strategy); \
	)

run-single-deck:
	$(foreach strategy,$(STRATEGIES), \
		$(MAKE) run-$(strategy)-single-deck; \
	)

run-double-deck:
	$(foreach strategy,$(STRATEGIES), \
		$(MAKE) run-$(strategy)-double-deck; \
	)

run-six-shoe:
	$(foreach strategy,$(STRATEGIES), \
		$(MAKE) run-$(strategy)-six-shoe; \
	)

# Aliases
rm: run-mimic
rm1: run-mimic-single-deck
rm2: run-mimic-double-deck
rm6: run-mimic-six-shoe

rl: run-linear
rl1: run-linear-single-deck
rl2: run-linear-double-deck
rl6: run-linear-six-shoe

rp: run-polynomial
rp1: run-polynomial-single-deck
rp2: run-polynomial-double-deck
rp6: run-polynomial-six-shoe

rn: run-neural
rn1: run-neural-single-deck
rn2: run-neural-double-deck
rn6: run-neural-six-shoe

rb: run-basic
rb1: run-basic-single-deck
rb2: run-basic-double-deck
rb6: run-basic-six-shoe

rh: run-high-low
rh1: run-high-low-single-deck
rh2: run-high-low-double-deck
rh6: run-high-low-six-shoe

rw: run-wong
rw1: run-wong-single-deck
rw2: run-wong-double-deck
rw6: run-wong-six-shoe

# Deck aliases
r1: run-single-deck
r2: run-double-deck
r6: run-six-shoe

