# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -I../include

# Source and target
SRC = client.c url.c file_utils.c
TARGET = client

# Default rule — build the program
all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS)

# Remove the compiled files
clean:
	rm -f $(TARGET)

# Test
run_test: $(TARGET)
	./$(TARGET) http://example.com/
