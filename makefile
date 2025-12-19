# Compiler and resource compiler
CC = gcc
WINDRES = windres

# Directories and files
SRCDIR = src
TARGET = Chess.exe
ICON_RC = icon.rc
ICON_RES = icon.res


# SDL includes
CFLAGS = -Wall -Wextra -DSDL_MAIN_HANDLED -std=c99\
-Iinclude \
-Ithirdparty/SDL2-2.24.0/x86_64-w64-mingw32/include/SDL2 \
-Ithirdparty/SDL2-2.24.0/x86_64-w64-mingw32/include \
-Ithirdparty/SDL2_image-2.7.1/x86_64-w64-mingw32/include/SDL2 \
-Ithirdparty/SDL2_image-2.7.1/x86_64-w64-mingw32/include \
-Ithirdparty/SDL2_mixer-2.8.1/x86_64-w64-mingw32/include/SDL2 \
-Ithirdparty/SDL2_mixer-2.8.1/x86_64-w64-mingw32/include \
-Ithirdparty/SDL2_ttf-2.24.0/x86_64-w64-mingw32/include/SDL2 \
-Ithirdparty/SDL2_ttf-2.24.0/x86_64-w64-mingw32/include 


# SDL libraries
LDFLAGS = \
-Lthirdparty/SDL2-2.24.0/x86_64-w64-mingw32/lib \
-Lthirdparty/SDL2_image-2.7.1/x86_64-w64-mingw32/lib \
-Lthirdparty/SDL2_mixer-2.8.1/x86_64-w64-mingw32/lib \
-Lthirdparty/SDL2_ttf-2.24.0/x86_64-w64-mingw32/lib

SDLLIBS = -lSDL2 -lSDL2main -lSDL2_image -lSDL2_mixer -lSDL2_ttf

SOURCES = \
src/chess.c \
src/DisplayConsole.c \
src/EndingConditions.c \
src/GameInitialization.c \
src/GameModeGui.c \
src/GuiHelpers.c \
src/InputConsole.c \
src/MainMenuGui.c \
src/MovingLogic.c \
src/SavingLoading.c \
src/StackHelpers.c

all: $(TARGET)



$(ICON_RES): $(ICON_RC)
	$(WINDRES) $(ICON_RC) -O coff -o $(ICON_RES)

$(TARGET): $(ICON_RES)
	$(CC) $(CFLAGS)  src/*.c $(ICON_RES) -o $(TARGET) $(LDFLAGS) $(SDLLIBS)
release: $(TARGET)



# Run  game
run: $(TARGET)
	powershell -Command "& './$(TARGET)'"


clean:
	rm -f Chess.exe icon.res 

.PHONY: all release  run  clean   
