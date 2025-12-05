#include "../include/stack.h"

Node* initializeStack()
{
    Node *stack = NULL;
    return stack;
}
void push(Node **stack,Game game)
{
    Node *temp =(Node*)malloc(sizeof(Node));
    temp->curGame = game;
    temp->nextGame = NULL;
    temp->nextGame = *stack;
    *stack = temp;
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
    }
    
}