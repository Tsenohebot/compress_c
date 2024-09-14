# Variables
CC = gcc
CFLAGS = -Wall -g   # Wall enables all warnings, and -g adds debugging information
TARGET = HUFFMAN
SRC = HUFFMAN.c
BUILD_DIR = build
TARGET_PATH = $(BUILD_DIR)/$(TARGET)

# Default target
all: $(BUILD_DIR) $(TARGET_PATH)

# Create the build directory if it doesn't exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Compile the source file to create the executable in the build directory
$(TARGET_PATH): $(SRC)
	$(CC) $(CFLAGS) -o $(TARGET_PATH) $(SRC)

# Clean up by removing the executable from the build directory
clean:
	rm -f $(TARGET_PATH)
