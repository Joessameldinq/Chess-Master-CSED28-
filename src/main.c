
#include "../include/definitions.h"
#include "../include/game.h"
#include "../include/display.h"
#ifdef _WIN32
#include <windows.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main()
{
    #ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);  // tell Windows console to use UTF-8
    #endif
    Game *game = initGame();
    displayWelcome();
    pause();
    clearScreen();
    displayHelp();
    pause();
    clearScreen();
    printGameState(game);
    pause();
    clearScreen();
    printBoard(game);
    pause();
    
}