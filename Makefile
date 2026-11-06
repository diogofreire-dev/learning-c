# Usage: make 01-basics/hello  (builds 01-basics/hello.c using the implicit rule)
CC      = clang
CFLAGS  = -std=c17 -Wall -Wextra -g -fsanitize=address,undefined
LDFLAGS = -fsanitize=address,undefined

.PHONY: help clean
help:
	@echo "Build an exercise:  make 01-basics/hello"
	@echo "Remove binaries:    make clean"

# Removes compiled programs (executables without an extension) from the topic folders
clean:
	find [0-9][0-9]-* -type f -perm -u+x ! -name '*.*' -print -delete
	find [0-9][0-9]-* -name '*.dSYM' -type d -prune -exec rm -rf {} +
