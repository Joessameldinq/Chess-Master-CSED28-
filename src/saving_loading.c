#include "../include/saving_loading.h"


bool undoMove(Node **gameStack, Node **redoStack)
{
    // Need at least 2 states to undo (current + previous)
    if (*gameStack == NULL || (*gameStack)->nextGame == NULL)
    {
        // printf("No moves to undo\n");will printed in the main game loop
        return false;
    }
    
    // Pop current state from game stack and push to redo stack
    Game currentGame = pop(gameStack);
    push(redoStack, currentGame);
    
    return true;
}

bool redoMove(Node **gameStack, Node **redoStack)
{
    if (isEmptyStack(*redoStack))
    {
        // printf("No moves to redo\n"); will printed in the game main loop
        return false;
    }
    
    // Pop from redo stack and push to game stack
    Game redoGame = pop(redoStack);
    push(gameStack, redoGame);
    
    return true;
}
bool saveGame(FILE *configuration)
{

}
bool loadGame(FILE *configuration)
{

}