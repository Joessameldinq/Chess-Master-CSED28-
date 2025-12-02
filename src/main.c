#include "../include/definitions.h"
#include "../include/game.h"
#include "../include/display.h"

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main()
{
    // 1. Start the music in the background (using mpg123)
    system("mpg123 'Erik Satie - Gnossienne No.1.mp3' &");

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

    // 2. Stop the music process by killing all running mpg123 processes
    printf("Stopping music...\n");
    system("pkill mpg123"); 

    return 0;
}
