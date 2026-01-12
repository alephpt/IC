# Makefile for Interaction Combinators (IC)

# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -pthread -O2
LDFLAGS = -pthread

# Source files
NODE_SRC = node.c
RULES_SRC = rules.c
PARALLEL_SRC = parallel.c
COMMON_SRC = $(NODE_SRC) $(RULES_SRC) $(PARALLEL_SRC)

# Object files
NODE_OBJ = $(NODE_SRC:.c=.o)
RULES_OBJ = $(RULES_SRC:.c=.o)
PARALLEL_OBJ = $(PARALLEL_SRC:.c=.o)
COMMON_OBJ = $(NODE_OBJ) $(RULES_OBJ) $(PARALLEL_OBJ)

# Executables
IC_MINIMAL = ic_minimal
IC_TEST = ic_test

# Default target
all: $(IC_MINIMAL) $(IC_TEST)

# Build ic_minimal
$(IC_MINIMAL): ic_minimal.o $(COMMON_OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

# Build ic_test
$(IC_TEST): ic_test.o $(COMMON_OBJ)
	$(CC) $(LDFLAGS) -o $@ $^

# Compile object files
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# Dependencies (headers)
node.o: node.c node.h
rules.o: rules.c rules.h node.h
parallel.o: parallel.c parallel.h rules.h node.h
ic_minimal.o: ic_minimal.c node.h rules.h parallel.h
ic_test.o: ic_test.c node.h rules.h parallel.h

# Run tests
test: $(IC_TEST)
	./$(IC_TEST)

# Run demo
demo: $(IC_MINIMAL)
	./$(IC_MINIMAL)

# Clean build artifacts
clean:
	rm -f $(COMMON_OBJ) ic_minimal.o ic_test.o $(IC_MINIMAL) $(IC_TEST)

# Clean everything including backup files
distclean: clean
	rm -f *~ *.bak

# Rebuild everything
rebuild: clean all

.PHONY: all test demo clean distclean rebuild
