#include "../include/input.h"
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdio.h>

#define MAX_INPUT_SIZE 50

bool isAlpahNumeric(char c)
{
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c >= '0' && c <= '9');
}

char* readInput(void)
{
    char *buffer = (char*)malloc(MAX_INPUT_SIZE + 1); // Caller must free it
       if (!buffer)  // Check malloc success
    {
        fprintf(stderr, "Memory allocation failed\n");
        return NULL;
    }
    
    char temp;
    int idx = 0;
    while ((temp = getchar()) != '\n' && temp != EOF)
    {
        if(isAlpahNumeric(temp) && idx < MAX_INPUT_SIZE)
            buffer[idx++] = temp;
    }
    buffer[idx] = '\0';
    return buffer;
}

bool validateInputFormat(char *input)
{
    if(strlen(input) < 4 || input ==NULL)
        return false;
    
    if(tolower(input[0]) < 'a' || tolower(input[0]) > 'h' || 
       tolower(input[2]) < 'a' || tolower(input[2]) > 'h')
        return false;
    
    if(input[1] < '1' || input[1] > '8' || 
       input[3] < '1' || input[3] > '8')
        return false;
    
    return true;
}

Position* parseMove(char *input)
{
    if(!input)
    {
        printf("Error\n");
        return NULL;
    }
    short fromX = '8' - input[1];
    short toX = '8' - input[3];
    short fromY = tolower(input[0]) - 'a';
    short toY = tolower(input[2]) - 'a';
    
    Position *pos = (Position*) malloc(2 * sizeof(Position));
    if (!pos)
        return NULL;
    
    pos[0] = (Position){.x = fromX, .y = fromY};
    pos[1] = (Position){.x = toX, .y = toY};
    return pos; // Caller must free pos
}

Piece getPromotion(Color color)
{
    printf("Enter Promotion Piece (By default Queen if Input is Invalid)\n");
    printf("(Q\\q)Queen (R\\r)Rook (B\\b)Bishop (N\\n)Knight\n");
    
    Piece p = (Piece){.color = color, .hasMoved = true, .type = QUEEN};
    char temp;
    char c;
    PieceType t;
    
    scanf(" %c", &temp);
    while ((c = getchar()) != '\n' && c != EOF);
    switch (tolower(temp))
    {
        case 'q':  t = QUEEN;  break;
        case 'b':  t = BISHOP; break;
        case 'n':  t = KNIGHT; break;
        case 'r':  t = ROOK;   break;   
        default:   t = QUEEN;  break;
    }
    p.type = t;
    return p;
}