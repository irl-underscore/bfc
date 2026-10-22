TEST_GFLAGS := -D _TEST=1 -D UNITY_OUTPUT_COLOR=1

TEST_CFLAGS = $(TEST_GFLAGS) -Isrc/

TEST_LFLAGS = $(TEST_GFLAGS)

TEST_SRC_DIR := test
TEST_BUILD_DIR := .test_build
TEST_OBJ_DIR := $(TEST_BUILD_DIR)/obj

TEST_SRCS := $(shell find "$(TEST_SRC_DIR)" -name "test_*.c")
TEST_OUTS := $(patsubst $(TEST_SRC_DIR)/test_%.c,$(TEST_BUILD_DIR)/test_%,$(TEST_SRCS))

UNITY_SRC = $(TEST_SRC_DIR)/unity/unity.c
UNITY_OBJ = $(TEST_BUILD_DIR)/unity.o

test-run: $(TEST_BUILD_DIR) $(TEST_OUTS)
	$(Q)$(foreach test,$(TEST_OUTS),./$(test) &&) true

$(TEST_BUILD_DIR)/test_%: $(TEST_SRC_DIR)/test_%.c $(UNITY_OBJ) $(C_OBJS) $(S_OBJS)
	@echo "LD\t$@"
	$(Q)$(LD) $^ -o $@ $(CFLAGS) $(LFLAGS)

$(UNITY_OBJ): $(UNITY_SRC)
	@echo "CC\t$@"
	$(Q)$(CC) -c $< -o $@ $(CFLAGS)

$(TEST_BUILD_DIR):
	$(Q)mkdir -p $@

clean-test:
	@echo "RM\t$(TEST_BUILD_DIR)"
	$(Q)rm -rf $(TEST_BUILD_DIR)
