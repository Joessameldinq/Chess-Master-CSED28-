#ifndef HELPERS_GUI_H
#define HELPERS_GUI_H

#include <SDL2/SDL.h>
#include "GuiDefinitions.h"
#include <stdbool.h>
int showExitConfirm(SDL_Window *window);
SDL_Texture *loadtexture(const char *file, SDL_Renderer *renderer);
bool isButtonClicked(int mx,int my,Button button);
int showSaveSlotDialog(SDL_Window *window);
void showGameMessage(SDL_Window *window, const char *title, const char *message);
void highlightClickedButton(SDL_Renderer *renderer,Button cButton);
void playSoundEffect(Mix_Chunk *sEffect);
void getWindowDimensions(SDL_Renderer *renderer, int *width, int *height);
int getSquareSize(void);
int getBoardOffset(void);
#endif