SHELL := bash

# ============================================================================
# Just Weather Client - C++ Makefile
# ============================================================================

# ------------------------------------------------------------
# Compilers
# ------------------------------------------------------------
CXX := g++
CC  := gcc

SRC_DIR := src

BUILD_MODE ?= debug
BUILD_DIR  := build/$(BUILD_MODE)
BIN        := $(BUILD_DIR)/just-weather-client

# ------------------------------------------------------------
# Build configuration
# ------------------------------------------------------------
ifeq ($(BUILD_MODE),release)
    CXXFLAGS_BASE := -O3 -DNDEBUG
    CFLAGS_BASE   := -O3 -DNDEBUG
    BUILD_TYPE    := Release
else
    CXXFLAGS_BASE := -O1 -g
    CFLAGS_BASE   := -O1 -g
    BUILD_TYPE    := Debug
endif

# ------------------------------------------------------------
# Include directories
# ------------------------------------------------------------
SRC_INCLUDES := $(shell find $(SRC_DIR) -type d 2>/dev/null)

INCLUDES := $(addprefix -I,$(SRC_INCLUDES))

# ------------------------------------------------------------
# Compiler flags
# ------------------------------------------------------------
CXXFLAGS := $(CXXFLAGS_BASE) -std=c++17 -Wall -Werror -Wfatal-errors -MMD -MP $(INCLUDES)
CFLAGS   := $(CFLAGS_BASE)   -w $(INCLUDES)

JANSSON_CFLAGS := $(CFLAGS) -Ilib/jansson

LDFLAGS :=
LIBS    := -ljansson

# ------------------------------------------------------------
# Source files
# ------------------------------------------------------------
CPP_SRC := $(shell find $(SRC_DIR) -type f -name '*.cpp')
C_SRC   := $(shell find $(SRC_DIR) -type f -name '*.c')

CPP_OBJ := $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/src/%.o,$(CPP_SRC))
C_OBJ   := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/src/%.o,$(C_SRC))

OBJ := $(CPP_OBJ) $(C_OBJ)
DEP := $(OBJ:.o=.d)

# ------------------------------------------------------------
# Build rules
# ------------------------------------------------------------
.PHONY: all
all: $(BIN)
	@echo "Build complete. [$(BUILD_TYPE)]"

$(BIN): $(OBJ)
	@echo "Linking client binary..."
	@mkdir -p $(dir $@)
	@$(CXX) $(LDFLAGS) $(OBJ) -o $@ $(LIBS)

# Compile C++ sources
$(BUILD_DIR)/src/%.o: $(SRC_DIR)/%.cpp
	@echo "Compiling C++ $<..."
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile C sources
$(BUILD_DIR)/src/%.o: $(SRC_DIR)/%.c
	@echo "Compiling C $<..."
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

# ------------------------------------------------------------
# Utilities
# ------------------------------------------------------------
.PHONY: clean
clean:
	@rm -rf build

.PHONY: interactive
interactive: $(BIN)
	@$(BIN) interactive

.PHONY: run
run: $(BIN)
	@$(BIN)

.PHONY: test-current
test-current: $(BIN)
	@echo "Testing current command with Kyiv coordinates..."
	@$(BIN) current 50.4501 30.5234

.PHONY: help
help:
	@echo "Available targets:"
	@echo "  make              - Build the project (debug mode)"
	@echo "  make clean        - Remove build artifacts"
	@echo "  make interactive  - Build and run in interactive mode"
	@echo "  make run          - Build and run (shows usage)"
	@echo "  make test-current - Build and test current command"
	@echo "  make BUILD_MODE=release - Build in release mode"

-include $(DEP)
