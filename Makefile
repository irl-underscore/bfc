MAKE_TEST := make-test

BUILD ?= debug
ARGS ?=

SRC_DIR := src
BUILD_DIR := build
DST_DIR := $(BUILD_DIR)/$(BUILD)
OBJ_DIR := $(DST_DIR)/obj
S_OBJ_DIR := $(OBJ_DIR)/asm
C_OBJ_DIR := $(OBJ_DIR)/c
BIN_DIR := bin
BIN_DST_DIR := $(BIN_DIR)/$(BUILD)
MAKE_TEST_DIR := $(BIN_DST_DIR)/$(MAKE_TEST)

G_FLAGS := -Wall -Wextra

CC := gcc
CFLAGS := $(G_FLAGS) --std=gnu99 -MMD -MP
C_DBGFLAGS := -D DEBUG=1 -g
C_RELFLAGS := -O3

AS := gcc
SFLAGS := $(G_FLAGS) -fPIE
S_DBGFLAGS := -D DEBUG=1 -g
S_RELFLAGS := -O3

LD := gcc
LFLAGS := $(G_FLAGS) -lm

ifeq ($(BUILD),debug)
	CFLAGS += $(C_DBGFLAGS)
	SFLAGS += $(S_DBGFLAGS)
else
	CFLAGS += $(C_RELFLAGS)
	SFLAGS += $(S_RELFLAGS)
endif

C_SRCS := $(shell find "$(SRC_DIR)" -name *.c)
C_OBJS := $(patsubst $(SRC_DIR)/%.c,$(C_OBJ_DIR)/%.o,$(C_SRCS))
S_SRCS := $(shell find "$(SRC_DIR)" -name *.s)
S_OBJS := $(patsubst $(SRC_DIR)/%.s,$(S_OBJ_DIR)/%.o,$(S_SRCS))

DEPS := $(C_OBJS:.o=.d) $(S_OBJS:.o=.d)

.PHONY: all debug release
all: $(MAKE_TEST_DIR)

dbg:
	$(MAKE) BUILD=debug

rel:
	$(MAKE) BUILD=release

run: $(MAKE_TEST_DIR)
	./$(MAKE_TEST_DIR) $(ARGS)

$(MAKE_TEST_DIR): $(S_OBJS) $(C_OBJS) | $(BIN_DST_DIR)
	$(LD) $^ -o $@ $(LFLAGS)

$(C_OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(C_OBJ_DIR)
	$(CC) -c $< -o $@ $(CFLAGS)

$(S_OBJ_DIR)/%.o: $(SRC_DIR)/%.s | $(S_OBJ_DIR)
	$(AS) -c $< -o $@ $(SFLAGS)

-include $(DEPS)
include install.mk

$(BIN_DST_DIR) $(C_OBJ_DIR) $(S_OBJ_DIR):
	mkdir -p $@

clean:
	rm -rf $(BUILD_DIR)
	rm -rf $(BIN_DIR)
