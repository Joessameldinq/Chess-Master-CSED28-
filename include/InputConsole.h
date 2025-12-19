#ifndef INPUT_H
#define INPUT_H
#include "GameDefinitions.h"
#include <stdbool.h>

bool validateInputFormat(char *input); //Input formats are e2 e4 e2-e4 E2E4 E2-E4 E2      E4
Position* parseMove(char *input); // Return two positions
Piece getPromotion(Color colro);
char* readInput(void);
bool isAlpahNumeric(char c);



#endif