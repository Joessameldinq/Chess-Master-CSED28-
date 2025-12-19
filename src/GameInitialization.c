

#include "../include/GameInitialization.h"
#include "../include/InputConsole.h"
#include <stdlib.h>
#include <string.h>
Game* initGame(void)
{
    Game* game = (Game*)malloc(sizeof(Game)); //don't forget to free it

    
    
    memset(game,0,sizeof(Game)); //As all bytes must be initialized to save and load  the game correctly   
    game->currentPlayer = WHITE;
    game->status = PLAYING;
    game->enPassentAvailable = false;
    game->enPassentTarget.x = -1;
    game->enPassentTarget.y = -1;
    for(int i = 0 ;i  < 8 ; i++)
    {
        game->capturedWhitePieces[i].color = WHITE;
        game->capturedWhitePieces[i].hasMoved = false;
        game->capturedWhitePieces[i].type = EMPTY;
        game->capturedBlackPieces[i].hasMoved = false;
        game->capturedBlackPieces[i].color = BLACK;
        game->capturedBlackPieces[i].type = EMPTY;
    }
    game->currentFlag.castlingMade =false;
    game->currentFlag.enpassentMade = false;
    game->currentFlag.pawnPromotionMade =false;


    game->halfMoveClock = 0;

    for(int i = 0 ; i < BOARD_SIZE ;i++)
    {
        
        for(int j = 0 ; j  < BOARD_SIZE ;j++)
        {
            game->board[i][j].hasMoved = false;
            if(i == 7 || i == 6)
                game->board[i][j].color = WHITE;
            else if(i == 0 || i == 1)
                game->board[i][j].color = BLACK;
            if(i== 0 || i == 7)
            {
                if(j == 0 || j == 7)
                game->board[i][j].type = ROOK;
                else if(j == 1 || j == 6)
                    game->board[i][j].type = KNIGHT;
                else if(j == 2 || j == 5)
                    game->board[i][j].type = BISHOP;
                else if(j == 3)
                    game->board[i][j].type = QUEEN;
                else 
                    game->board[i][j].type = KING;
            }
            else if( i == 1 || i == 6)
                game->board[i][j].type = PAWN;
            else
                {game->board[i][j].type = EMPTY;
                game->board[i][j].color = NONE;}

        }
    }


    return game;
    //Caller free game

}