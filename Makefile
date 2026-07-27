ifeq ($(OS), Windows_NT)
    MKDIR = if not exist "$(subst /,\,$(1))" mkdir "$(subst /,\,$(1))"
    CLEAN = if exist "$(subst /,\,$(1))" rd /s /q "$(subst /,\,$(1))"
    EXE := .exe
else
    MKDIR = mkdir -p $(1)
    CLEAN = rm -rf $(1)
    EXE :=
endif

RWILDCARD = $(foreach d,$(wildcard $(1:=/*)),$(call RWILDCARD,$d,$2) $(filter $(subst *,%,$2),$d))

MAJOR := 1
MINOR := 1
PATCH := 2
PROJECT := bfc

CC ?= clang
STRIP := strip
CFLAGS := -Wall -Wextra -std=gnu99 -MMD -DVERSION_MAJOR=$(MAJOR) -DVERSION_MINOR=$(MINOR) -DVERSION_PATCH=$(PATCH) -DPROJECT=\"$(PROJECT)\"
RELFLAGS := -O3
DBFLAGS := -g -DDEBUG=1

OBJ_EXTENSION := o
DEBUG_DIR := debug
DEBUG_OBJ_DIR := $(DEBUG_DIR)/obj
RELEASE_DIR := release
RELEASE_OBJ_DIR := $(RELEASE_DIR)/obj
SRC_DIR := src

SRCS := $(call RWILDCARD,$(SRC_DIR),*.c)
DEBUG_OBJS := $(patsubst $(SRC_DIR)/%.c, $(DEBUG_OBJ_DIR)/%.o, $(SRCS))
RELEASE_OBJS := $(patsubst $(SRC_DIR)/%.c, $(RELEASE_OBJ_DIR)/%.o, $(SRCS))

DEBUG := $(DEBUG_DIR)/$(PROJECT)
RELEASE := $(RELEASE_DIR)/$(PROJECT)

.PHONY: all clean rel run test

all: $(DEBUG)

re: clean $(DEBUG)

$(DEBUG): $(DEBUG_OBJS) | $(DEBUG_DIR)
	$(CC) $(CFLAGS) $(DBFLAGS) $^ -o $@

rel: $(RELEASE_OBJS) | $(RELEASE_DIR)
	$(CC) $(CFLAGS) $(RELFLAGS) $^ -o $(RELEASE)
	$(STRIP) $(RELEASE)$(EXE)

$(DEBUG_OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(DEBUG_OBJ_DIR)
	$(call MKDIR,$(dir $@))
	$(CC) $(CFLAGS) $(DBFLAGS) -c $< -o $@

$(RELEASE_OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(RELEASE_OBJ_DIR)
	$(call MKDIR,$(dir $@))
	$(CC) $(CFLAGS) $(RELFLAGS) -c $< -o $@

$(DEBUG_DIR) $(RELEASE_DIR) $(DEBUG_OBJ_DIR) $(RELEASE_OBJ_DIR):
	$(call MKDIR,$@)

clean:
	$(call CLEAN,$(DEBUG_DIR))
	$(call CLEAN,$(RELEASE_DIR))
