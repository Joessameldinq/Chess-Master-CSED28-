CC := gcc
CFLAGS := -Wall -Wextra -Iinclude

# Directories
SRC_DIR := src
INC_DIR := include
OBJ_DIR := obj
BIN_DIR := bin

# Target
TARGET := $(BIN_DIR)/tictactoe
SRCS := $(SRC_DIR)/main.c $(SRC_DIR)/game.c $(SRC_DIR)/display.c
OBJS := $(OBJ_DIR)/main.o $(OBJ_DIR)/game.o $(OBJ_DIR)/display.o

# Default target
all: directories $(TARGET)

# Create necessary directories
directories:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(BIN_DIR)

# Link object files to create executable
$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET)
	@echo "Build complete with no errors"

# Compile source files to object files
$(OBJ_DIR)/main.o: $(SRC_DIR)/main.c $(INC_DIR)/game.h $(INC_DIR)/display.h $(INC_DIR)/definitions.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/game.o: $(SRC_DIR)/game.c $(INC_DIR)/game.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/display.o: $(SRC_DIR)/display.c $(INC_DIR)/display.h
	$(CC) $(CFLAGS) -c $< -o $@

# Clean
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)
	@echo "Clean complete"

# Run
run: all
	./$(TARGET)
