#ifndef STACK_H
#define STACK_H

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include "GameDefinitions.h"

typedef struct Node
{
    Game curGame;
    struct Node* nextGame;
}Node;

Node* initializeStack();
bool push(Node **stack,Game game);
Game pop(Node **stack);
bool isEmptyStack(Node *stack);
void clearStack(Node **stack);


#endif