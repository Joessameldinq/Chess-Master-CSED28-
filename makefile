# Compiler and resource compiler
CC = gcc
WINDRES = windres

# Directories and files
GUI_TARGET = ChessGUI.exe
CONSOLE_TARGET = ChessConsole.exe
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




all: $(GUI_TARGET) $(ChessGUI)



$(ICON_RES): $(ICON_RC)
	@ $(WINDRES) $(ICON_RC) -O coff -o $(ICON_RES)
	@ echo "Icon built sucessfully"


$(GUI_TARGET): $(ICON_RES)
	@ $(CC) $(CFLAGS)  src/Common/*.c src/Gui/*.c $(ICON_RES) -o $(GUI_TARGET) $(LDFLAGS) $(SDLLIBS)
	@ echo "Gui exe built sucessfully"
$(CONSOLE_TARGET): $(ICON_RES)
	@ $(CC) $(CFLAGS)  src/Common/*.c src/Console/*.c $(ICON_RES) -o $(CONSOLE_TARGET) 
	@ echo "Console exe built sucessfully"


# Run  game
run-GUI: $(GUI_TARGET)
	@ echo "Gui running"
	@ ./$(GUI_TARGET)
	@ echo "Game closed Thank You for playing"
run-Console: $(CONSOLE_TARGET)
	@ echo "Console running"
	@ ./$(CONSOLE_TARGET)
	@ echo "Game closed Thank You for playing"


clean:
	@ rm -f ChessGUI.exe ChessConsole.exe icon.res 
	@ echo "Clean files"

.PHONY: all   run-GUI run-Console  clean