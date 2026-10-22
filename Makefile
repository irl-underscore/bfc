MAKE_TEST := bfc

BUILD ?= debug
ARGS ?=

SRC_DIR := src
BUILD_DIR := build
DST_DIR := $(BUILD_DIR)/$(BUILD)
OBJ_DIR := $(DST_DIR)/obj
BIN_DIR := bin
BIN_DST_DIR := $(BIN_DIR)/$(BUILD)
MAKE_TEST_DIR := $(BIN_DST_DIR)/$(MAKE_TEST)

G_FLAGS := -Wall -Wextra

CC := gcc
CFLAGS = $(G_FLAGS) --std=gnu99 -MMD -MP
C_DBGFLAGS := -D DEBUG=1 -g
C_RELFLAGS := -O3
C_TESTFLAGS := -D _TEST=1 -D UNITY_OUTPUT_COLOR=1 -I./src/

AS := gcc
SFLAGS = $(G_FLAGS) -fPIE
S_DBGFLAGS := -D DEBUG=1 -g
S_RELFLAGS := -O3

LD := gcc
LFLAGS = $(G_FLAGS) -lm

ifeq ($(BUILD),debug)
	CFLAGS += $(C_DBGFLAGS)
	SFLAGS += $(S_DBGFLAGS)
else ifeq ($(BUILD),test)
	CFLAGS += $(C_TESTFLAGS)
else
	CFLAGS += $(C_RELFLAGS)
	SFLAGS += $(S_RELFLAGS)
endif

C_SRCS := $(shell find "$(SRC_DIR)" -name *.c)
C_OBJS := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.c.o,$(C_SRCS))
S_SRCS := $(shell find "$(SRC_DIR)" -name *.s)
S_OBJS := $(patsubst $(SRC_DIR)/%.s,$(OBJ_DIR)/%.s.o,$(S_SRCS))

DEPS := $(C_OBJS:.o=.d) $(S_OBJS:.o=.d)

P ?= 0
ifeq ($(P),0)
	Q := @
else
	Q :=
endif

.PHONY: all debug release test
all: $(MAKE_TEST_DIR)

dbg:
	$(Q)$(MAKE) BUILD=debug

rel:
	$(Q)$(MAKE) BUILD=release

test:
	$(Q)$(MAKE) test-run BUILD=test

run: $(MAKE_TEST_DIR)
	@echo "RUN\t$<"
	$(Q)./$(MAKE_TEST_DIR) $(ARGS)

$(MAKE_TEST_DIR): $(S_OBJS) $(C_OBJS) | $(BIN_DST_DIR)
	@echo "LD\t$@"
	$(Q)$(LD) $^ -o $@ $(LFLAGS)
ifeq ($(BUILD),release)
	$(Q)strip $@
endif

$(OBJ_DIR)/%.c.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@echo "CC\t$@"
	$(Q)$(CC) -c $< -o $@ $(CFLAGS)

# i dont konw why i put as there but ill keep it :)
$(OBJ_DIR)/%.s.o: $(SRC_DIR)/%.s | $(OBJ_DIR)
	@echo "AS\t$@"
	$(Q)$(AS) -c $< -o $@ $(SFLAGS)

-include $(DEPS)
include test/test.mk
include install.mk

$(BIN_DST_DIR) $(OBJ_DIR):
	$(Q)mkdir -p $@

clean:
	@echo "RM\t$(BUILD_DIR)"
	$(Q)rm -rf $(BUILD_DIR)
	@echo "RM\t$(BIN_DIR)"
	$(Q)rm -rf $(BIN_DIR)

clean-all: clean clean-test
