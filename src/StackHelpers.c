




#include "../include/StackHelpers.h"

Node* initializeStack()
{
    Node *stack = NULL;
    return stack;
}
bool push(Node **stack,Game game)
{
    Node *temp =(Node*)malloc(sizeof(Node));
    temp->curGame = game;
    temp->nextGame = NULL;
    temp->nextGame = *stack;
    *stack = temp;
    return true;
}
Game pop(Node **stack)
{
    if(isEmptyStack(*stack))
        {
            printf("The stack is empty\n");
            return (Game){0};
        }
    Node *temp = *stack;
    *stack = (*stack)->nextGame;
    Game gm = temp->curGame;
    free(temp);
    temp =NULL;
    return gm;
}
bool isEmptyStack(Node *stack)
{
    if(stack == NULL)
        return true;
    return false;
}
void clearStack(Node **stack)
{
    while (*stack != NULL)
    {
        Node *temp = *stack;
        *stack = (*stack)->nextGame;
        free(temp);
        temp = NULL;
    }
    
}