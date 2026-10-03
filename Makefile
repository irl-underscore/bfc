BFC := bfc
LIB_BF := bf_core
LIB_BF_NAME := libbf_core.a

GFLAGS := -Wall -Wextra
GDFLAGS := -DVER_MIN=1 -DVER_MAJOR=1 -DVER_PATCH=1 -DPNAME=\"$(TARGET)\"

CC = gcc
CFLAGS = $(GFLAGS) $(GDFLAGS) -std=gnu99 -MMD -MP -Ibf_core/include/
DEBUG_FLAGS := -DEBUG=1 -g
RELEASE_FLAGS := -O3
TEST_FLAGS := $(DEBUG_FLAGS) -DTEST=1

LD = gcc
LFLAGS = $(GFLAGS) -lm -L$(LIB_DIR) -l$(LIB_BF)

AR = ar
ARFLAGS = rcs

P ?= 0
ifeq ($(P), 1)
	Q :=
else
	Q := @
endif

SRC_DIR = src
LIB_SRC_DIR = bf_core/src
BUILD_DIR = build
BIN_DIR = bin
LIB_DIR = $(BIN_DIR)/lib
OBJ_DIR = $(BUILD_DIR)/obj

SRCS = $(shell find "$(SRC_DIR)" -name "*.c")
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
LIB_SRCS = $(shell find "$(LIB_SRC_DIR)" -name "*.c")
LIB_OBJS = $(patsubst $(LIB_SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(LIB_SRCS))
DEPS = $(OBJS:.o=.d)

all: debug

debug: _invoke_debug_flags _compile
release: _invoke_release_flags _compile

_compile: clean $(LIB_DIR)/$(LIB_BF_NAME) $(BIN_DIR)/$(BFC)

$(BIN_DIR)/$(BFC): $(OBJS)
	@echo "LD\t$@"
	$(Q)mkdir -p $(dir $@)
	$(Q)$(LD) $^ -o $@ $(LFLAGS)
ifeq ($(REL), 1)
	$(Q)strip $@
endif

$(LIB_DIR)/$(LIB_BF_NAME): $(LIB_OBJS)
	@echo "AR\t$@"
	$(Q)mkdir -p $(dir $@)
	$(Q)$(AR) $(ARFLAGS) $@ $^
ifeq ($(REL), 1)
	$(Q)strip $@
endif

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@echo "CC\t$<"
	$(Q)mkdir -p $(dir $@)
	$(Q)$(CC) -c $< -o $@ $(CFLAGS)

$(OBJ_DIR)/%.o: $(LIB_SRC_DIR)/%.c
	@echo "CC\t$<"
	$(Q)mkdir -p $(dir $@)
	$(Q)$(CC) -c $< -o $@ $(CFLAGS)

-include $(DEPS)
include install.mk

.PHONY: _invoke_debug_flags _invoke_release_flags
_invoke_debug_flags:
	$(eval CFLAGS += $(DEBUG_FLAGS))
	$(eval LFLAGS += $(DEBUG_FLAGS))

_invoke_release_flags:
	$(eval CFLAGS += $(RELEASE_FLAGS))
	$(eval LFLAGS += $(RELEASE_FLAGS))

.PHONY: run
run: $(BIN_DIR)/$(BFC)
	@echo "RUN\t$<"
	$(Q)./$(BIN_DIR)/$(BFC)

clean:
	@echo "RM\t$(BUILD_DIR)"
	$(Q)rm -rf $(BUILD_DIR)
	@echo "RM\t$(BIN_DIR)"
	$(Q)rm -rf $(BIN_DIR)
