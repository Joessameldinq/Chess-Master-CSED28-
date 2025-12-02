#include "../include/definitions.h"
#include "../include/game.h"
#include "../include/display.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main()
{
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