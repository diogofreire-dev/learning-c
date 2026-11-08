# Usage:
#   make 01-basics/02-calc        builds 01-basics/02-calc.c into build/01-basics/02-calc
#   make run/01-basics/02-calc    builds it and runs it
#   make run/05-structs-files/03-args ARGS="1 2 3"   runs it with command-line arguments
#   make leaks/04-memory/05-leak  builds it without sanitizers and checks it for memory leaks
#   make all                      builds every exercise
#   make clean                    removes build/
CC      = clang
CFLAGS  = -std=c17 -Wall -Wextra -g -fsanitize=address,undefined
LDFLAGS = -fsanitize=address,undefined
BUILD   = build

# Every exercise, e.g. 01-basics/02-calc (the .c files without the extension)
PROGS = $(patsubst %.c,%,$(wildcard [0-9][0-9]-*/*.c))
RUNS  = $(addprefix run/,$(PROGS))
LEAKS = $(addprefix leaks/,$(PROGS))

# No built-in rules: otherwise make would build next to the source (cc file.c -o file)
MAKEFLAGS += -r
.SUFFIXES:

.PHONY: help all clean $(PROGS) $(RUNS) $(LEAKS)
help:
	@echo "Build an exercise:  make 01-basics/02-calc   (output in build/)"
	@echo "Build and run it:   make run/01-basics/02-calc"
	@echo "With arguments:     make run/05-structs-files/03-args ARGS=\"1 2 3\""
	@echo "Check for leaks:    make leaks/04-memory/05-leak"
	@echo "Build everything:   make all"
	@echo "Remove binaries:    make clean"

all: $(PROGS)

# Also removes binaries left next to the sources (e.g. built by the VS Code run button)
clean:
	rm -rf $(BUILD)
	find [0-9][0-9]-* -type f -perm -u+x ! -name '*.*' -print -delete
	find [0-9][0-9]-* -name '*.dSYM' -type d -prune -exec rm -rf {} +

# Without sanitizers: AddressSanitizer cannot find leaks on macOS, and Apple's leaks
# tool does not work on a program built with it
$(BUILD)/plain/%: %.c
	@mkdir -p $(dir $@)
	$(CC) -std=c17 -Wall -Wextra -g $< -o $@

$(BUILD)/%: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $< -o $@ $(LDFLAGS)

# make 01-basics/02-calc → build/01-basics/02-calc
$(PROGS): %: $(BUILD)/%

# The leading - keeps make going if the program exits with an error code
$(RUNS): run/%: $(BUILD)/%
	-@./$< $(ARGS)

# MallocStackLogging makes leaks show where each leaked block was allocated
$(LEAKS): leaks/%: $(BUILD)/plain/%
	-@MallocStackLogging=1 leaks --atExit -- ./$< $(ARGS)
