CC := gcc
MKDIR := mkdir -p

SRC_DIR := src/ci
INC_DIR := include/ci
OBJ_DIR := src/ci
BIN_DIR := bin
TEST_DIR := testcases

SRCS := $(shell find $(SRC_DIR) -name '*.c')
OBJS := $(SRCS:%.c=%.o)

CFLAGS := -I$(INC_DIR) \
          -std=c11 \
          -Wall \
          -Wextra \
          -Wpedantic \
          -Werror \
          -Wformat=2 \
          -Wshadow \
          -Wwrite-strings \
          -Wstrict-prototypes \
          -Wold-style-definition \
          -Wredundant-decls \
          -Wnested-externs \
          -Wmissing-include-dirs \
          -Wjump-misses-init \
          -Wlogical-op \
          -Wduplicated-cond \
          -Wduplicated-branches \
          -Wformat-security \
          -Wformat-signedness \
          -Wimplicit-fallthrough=5 \
          -fstack-protector-strong \
          -Wno-unused-function \
          -Wno-unused-parameter

RELEASE_FLAGS := -O2

DEBUG_FLAGS := -g3 -DDEBUG -O0

WEEK2_TESTS := $(wildcard $(TEST_DIR)/week2/*)
WEEK3_TESTS := $(wildcard $(TEST_DIR)/week3/*)
WEEK4_TESTS := $(wildcard $(TEST_DIR)/week4/*)

VALGRIND := valgrind
VALGRIND_FLAGS := --error-exitcode=1 --leak-check=full --show-leak-kinds=all --track-origins=yes

.PHONY: all
all: CFLAGS += $(RELEASE_FLAGS)
all: $(BIN_DIR)/ci

.PHONY: test_week2
test_week2: $(BIN_DIR)/ci
	@echo "Running Week 2 tests..."
	@for test in $(WEEK2_TESTS); do \
		echo "\nTesting $$test:"; \
		$(BIN_DIR)/ci -i $$test; \
	done

	@echo "\nRunning Week 2 tests with Valgrind..."
	@for test in $(WEEK2_TESTS); do \
		echo "\nValgrind check for $$test:"; \
		$(VALGRIND) $(VALGRIND_FLAGS) $(BIN_DIR)/ci -i $$test; \
	done

.PHONY: test_week3
test_week3: $(BIN_DIR)/ci
	@echo "Running Week 3 tests..."
	@for test in $(WEEK3_TESTS); do \
		echo "\nTesting $$test:"; \
		$(BIN_DIR)/ci -i $$test; \
	done

	@echo "\nRunning Week 3 tests with Valgrind..."
	@for test in $(WEEK3_TESTS); do \
		echo "\nValgrind check for $$test:"; \
		$(VALGRIND) $(VALGRIND_FLAGS) $(BIN_DIR)/ci -i $$test; \
	done

.PHONY: test_week4
test_week4: $(BIN_DIR)/ci
	@echo "Running Week 4 tests..."
	@for test in $(WEEK4_TESTS); do \
		echo "\nTesting $$test:"; \
		$(BIN_DIR)/ci -i $$test; \
	done

	@echo "\nRunning Week 4 tests with Valgrind..."
	@for test in $(WEEK4_TESTS); do \
		echo "\nValgrind check for $$test:"; \
		$(VALGRIND) $(VALGRIND_FLAGS) $(BIN_DIR)/ci -i $$test; \
	done


.PHONY: debug
debug: CFLAGS += $(DEBUG_FLAGS)
debug: $(BIN_DIR)/ci

$(BIN_DIR):
	$(MKDIR) $(BIN_DIR)

$(BIN_DIR)/ci: $(OBJS) | $(BIN_DIR)
	$(CC) $(OBJS) $(CFLAGS) -o $@

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -f $(OBJS) $(BIN_DIR)/ci
	rm -rf $(BIN_DIR)
