ifeq ($(OS), Windows_NT)
    MKDIR = if not exist "$(subst /,\,$(1))" mkdir "$(subst /,\,$(1))"
    MKFILE = type nul > "$(subst /,\,$(1))"
    CLEAN = if exist "$(subst /,\,$(1))" rd /s /q "$(subst /,\,$(1))"
    EXE := .exe
else
    MKDIR = mkdir -p $(1)
    MKFILE = touch $(1)
    CLEAN = rm -rf $(1)
    EXE :=
endif

CC = clang
CFLAGS = -Wall -Wextra -std=c99
RELFLAGS = -O3
DBFLAGS = -Od

BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
DEBUG_DIR = $(BUILD_DIR)/debug
SRC_DIR = src
TEST_DIR = test

SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRCS))

PROJECT = brainfuck
PROJECT_BAT = $(TEST_DIR)/$(PROJECT).bat

DEBUG = $(DEBUG_DIR)/$(PROJECT)
RELEASE = $(BUILD_DIR)/$(PROJECT)

.PHONY: all clean release

all: $(DEBUG)

$(DEBUG): $(OBJS) | $(DEBUG_DIR)
	$(CC) $(CFLAGS) $(DBFLAGS) $^ -o $@

release: $(OBJS) | $(BUILD_DIR)
	$(CC) $(CFLAGS) $(RELFLAGS) $^ -o $(RELEASE)

$(PROJECT_BAT): | $(TEST_DIR)
	$(call MKFILE,$@)
	@echo @echo off > $@
	@echo "%~dp0$(subst /,\,$(DEBUG))$(EXE)" %%* >> $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR) $(DEBUG_DIR) $(OBJ_DIR) $(TEST_DIR):
	$(call MKDIR,$@)

run: $(DEBUG)
	@$(subst /,\,$(DEBUG))$(EXE) main.bf  & echo Program finished with exit code: %errorlevel%

clean:
	$(call CLEAN,$(BUILD_DIR))
