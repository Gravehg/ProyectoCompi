# Compiler settings
CC = gcc
CFLAGS = -w

# Source files
SRC_FILES = scanner.c parser.c buffer.c main.c semantics.c

# Output binary
OUTPUT = compileMicro

# OUTPUT
all: $(OUTPUT)

# Command
$(OUTPUT): $(SRC_FILES)
	$(CC) $(CFLAGS) $(SRC_FILES) -o $(OUTPUT)

# Clean
clean:
	rm -f $(OUTPUT)