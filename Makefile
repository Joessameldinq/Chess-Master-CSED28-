# --------------------
# Compiler and flags
# --------------------
CC = gcc
CFLAGS = -Wall -Wextra -std=c99 -O2 -I./include
LDFLAGS =

# --------------------
# Directories
# --------------------
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
BIN_DIR = bin

# --------------------
# Detect OS
# --------------------
ifeq ($(OS),Windows_NT)
    EXE = .exe
    WINDRES_EXISTS := yes
else
    EXE =
    WINDRES_EXISTS := no
endif

# --------------------
# Target executable
# --------------------
TARGET = $(BIN_DIR)/Chess$(EXE)

# --------------------
# Source and object files
# --------------------
SRCS = $(wildcard $(SRC_DIR)/*.c)
OBJS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

# --------------------
# Default target
# --------------------
all: directories $(TARGET)

# --------------------
# Create necessary directories
# --------------------
directories:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(BIN_DIR)

# --------------------
# Compile .c to .o
# --------------------
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# --------------------
# Windows icon (only on Windows)
# --------------------
ifeq ($(WINDRES_EXISTS),yes)
ICON_OBJ = $(OBJ_DIR)/icon.o
$(ICON_OBJ): icon.rc icon.ico
	windres icon.rc -O coff -o $(ICON_OBJ)
else
ICON_OBJ =
endif

# --------------------
# Link executable
# --------------------
$(TARGET): $(OBJS) $(ICON_OBJ)
	$(CC) $(OBJS) $(ICON_OBJ) -o $(TARGET)
	@echo "Build complete: $(TARGET)"

# --------------------
# Clean
# --------------------
clean:
	@rm -rf $(OBJ_DIR) $(BIN_DIR)
	@echo "Clean complete"

# --------------------
# Run
# --------------------
run: all
ifeq ($(OS),Windows_NT)
	$(TARGET)
else
	./$(TARGET)
endif

# --------------------
# Windows explicit cross-build
# --------------------
windows: directories $(OBJS)
	# Compile Windows resources
	x86_64-w64-mingw32-windres icon.rc -O coff -o $(OBJ_DIR)/icon.o
	# Compile all C files to Windows object files
	x86_64-w64-mingw32-gcc $(CFLAGS) -c src/display.c -o $(OBJ_DIR)/display_win.o
	x86_64-w64-mingw32-gcc $(CFLAGS) -c src/end.c -o $(OBJ_DIR)/end_win.o
	x86_64-w64-mingw32-gcc $(CFLAGS) -c src/input.c -o $(OBJ_DIR)/input_win.o
	x86_64-w64-mingw32-gcc $(CFLAGS) -c src/move.c -o $(OBJ_DIR)/move_win.o
	x86_64-w64-mingw32-gcc $(CFLAGS) -c src/saving_loading.c -o $(OBJ_DIR)/saving_loading_win.o
	x86_64-w64-mingw32-gcc $(CFLAGS) -c src/stack.c -o $(OBJ_DIR)/stack_win.o
	x86_64-w64-mingw32-gcc $(CFLAGS) -c src/game.c    -o $(OBJ_DIR)/game_win.o
	x86_64-w64-mingw32-gcc $(CFLAGS) -c src/main.c    -o $(OBJ_DIR)/main_win.o
	x86_64-w64-mingw32-gcc $(CFLAGS) -c src/utilities.c    -o $(OBJ_DIR)/utilities_win.o
	# Link everything into a Windows executable
	x86_64-w64-mingw32-gcc $(OBJ_DIR)/end_win.o $(OBJ_DIR)/input_win.o $(OBJ_DIR)/utilities_win.o  $(OBJ_DIR)/saving_loading_win.o $(OBJ_DIR)/stack_win.o $(OBJ_DIR)/move_win.o $(OBJ_DIR)/display_win.o $(OBJ_DIR)/game_win.o $(OBJ_DIR)/main_win.o $(OBJ_DIR)/icon.o -o $(BIN_DIR)/Chess.exe
	@echo "Windows .exe build complete: $(BIN_DIR)/Chess.exe"

.PHONY: all clean run directories windows
