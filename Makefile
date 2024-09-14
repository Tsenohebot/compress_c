# Variables
CC = gcc
CFLAGS = -Wall -g   # Wall enables all warnings, and -g adds debugging information
TARGET = HUFFMAN
SRC = HUFFMAN.c

# Default target
all: $(TARGET)

# Compile the source file to create the executable
$(TARGET): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC)

# Clean up by removing the executable
clean:
	rm -f $(TARGET)
