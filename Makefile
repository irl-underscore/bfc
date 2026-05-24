ifeq ($(OS), Windows_NT)
    MKDIR = if not exist "$(subst /,\,$(1))" mkdir "$(subst /,\,$(1))"
    CLEAN = if exist "$(subst /,\,$(1))" rd /s /q "$(subst /,\,$(1))"
    EXE := .exe
    RUN_CMD = $(subst /,\,$(DEBUG))$(EXE) main.bf & echo Program finished with exit code: %errorlevel%
else
    MKDIR = mkdir -p $(1)
    CLEAN = rm -rf $(1)
    EXE :=
    RUN_CMD = ./$(DEBUG) main.bf && echo Program finished.
endif

CC := clang
CFLAGS := -Wall -Wextra -std=c99
RELFLAGS := -O3
DBFLAGS := -g -DDEBUG=1

OBJ_EXTENSION := o
DEBUG_DIR := debug
DEBUG_OBJ_DIR := $(DEBUG_DIR)/obj
RELEASE_DIR := release
RELEASE_OBJ_DIR := $(RELEASE_DIR)/obj
SRC_DIR := src

SRCS := $(wildcard $(SRC_DIR)/*.c)
DEBUG_OBJS := $(patsubst $(SRC_DIR)/%.c, $(DEBUG_OBJ_DIR)/%.o, $(SRCS))
RELEASE_OBJS := $(patsubst $(SRC_DIR)/%.c, $(RELEASE_OBJ_DIR)/%.o, $(SRCS))
PROJECT := brainfuck

DEBUG := $(DEBUG_DIR)/$(PROJECT)
RELEASE := $(RELEASE_DIR)/$(PROJECT)

.PHONY: all clean rel run

all: $(DEBUG)

$(DEBUG): $(DEBUG_OBJS) | $(DEBUG_DIR)
	$(CC) $(CFLAGS) $(DBFLAGS) $^ -o $@

rel: $(RELEASE_OBJS) | $(RELEASE_DIR)
	$(CC) $(CFLAGS) $(RELFLAGS) $^ -o $(RELEASE)

$(DEBUG_OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(DEBUG_OBJ_DIR)
	$(CC) $(CFLAGS) $(DBFLAGS) -c $< -o $@

$(RELEASE_OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(RELEASE_OBJ_DIR)
	$(CC) $(CFLAGS) $(RELFLAGS) -c $< -o $@

$(DEBUG_DIR) $(RELEASE_DIR) $(DEBUG_OBJ_DIR) $(RELEASE_OBJ_DIR):
	$(call MKDIR,$@)

run: $(DEBUG)
	@$(RUN_CMD)

clean:
	$(call CLEAN,$(DEBUG_DIR))
	$(call CLEAN,$(RELEASE_DIR))
