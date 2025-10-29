# Compiler and flags
CC = gcc
CFLAGS = -Wall -Wextra -g -I../include

# Source and target
SRC = server.c mime.c
TARGET = server

# Default rule — build the program
all: $(TARGET)

$(TARGET): $(SRC)
	$(CC) $(SRC) -o $(TARGET) $(CFLAGS)

# Remove the compiled files
clean:
	rm -f $(TARGET)
