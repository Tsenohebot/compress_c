# Detect system architecture
ARCH := $(shell uname -m)

# Set default compilers for ARM and x86
CC_x86 := gcc
CC_arm := arm-linux-gnueabi-gcc
CC_arm32 := arm-linux-gnueabi-gcc


# Select compiler based on architecture
ifeq ($(ARCH),x86_64)
    CC := $(CC_x86)
else ifeq ($(ARCH),aarch64)
    CC := $(CC_arm)
else ifeq ($(ARCH),armv7l)
    CC := $(CC_arm32)
else
    $(error Unsupported architecture: $(ARCH))
endif

CFLAGS = -Wall -g   # Wall enables all warnings, and -g adds debugging information
TARGET = HUFFMAN
SRC = src/HUFFMAN.c
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
# Nuke build folder - Only do this to purge contamination or incorrectly built binary
nuke:
	rm -rf $(BUILD_DIR)
