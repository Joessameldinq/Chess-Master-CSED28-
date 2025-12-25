


#include "../include/GuiHelpers.h"
#include <stdbool.h>

// Global cache for dynamic dimensions (updated from GameModeGui)
extern int g_squareSize;
extern int g_boardOffset;

// Helper to get current window dimensions for resize support
void getWindowDimensions(SDL_Renderer *renderer, int *width, int *height) {
    SDL_Window *window = SDL_RenderGetWindow(renderer);
    if (window) {
        SDL_GetWindowSize(window, width, height);
    }
}

// Helper to get current square size
int getSquareSize() {
    return g_squareSize;
}

// Helper to get current board offset
int getBoardOffset() {
    return g_boardOffset;
}

SDL_Texture *loadtexture(const char *file, SDL_Renderer *renderer)
{
    SDL_Surface *surface = IMG_Load(file);
    if (!surface)
    {
        SDL_Log("Unable to load image %s! SDL_image Error: %s\n", file, IMG_GetError());
        return NULL;
    }
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    if (!texture)
    {
        SDL_Log("Unable to create texture from %s! SDL Error: %s\n", file, SDL_GetError());
        return NULL;
    }
    return texture;
}
bool isButtonClicked(int mx,int my,Button b)
{
    if(mx >= b.rect.x && mx <= b.rect.x + b.rect.w &&
       my >= b.rect.y && my <= b.rect.y + b.rect.h)
        return true;  
              
    return false;   
}
int showExitConfirm(SDL_Window *window)
{
    const SDL_MessageBoxButtonData buttons[] = {
        { 0, 1, "Yes" },
        { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 2, "No" }
    };

    const SDL_MessageBoxData data = {
        SDL_MESSAGEBOX_WARNING,
        window,
        "Exit Game",
        "Are you sure you want to quit?",
        SDL_arraysize(buttons),
        buttons,
        NULL
    };

    int id;
    SDL_ShowMessageBox(&data, &id);
    return id; // 1 = yes, 2 = no
}
int showSaveSlotDialog(SDL_Window *window)
{
    const SDL_MessageBoxButtonData buttons[] = {
        { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 1, "Game 1" },
        { 0, 2, "Game 2" },
        { 0, 3, "Game 3" },
        { 0, 4, "Game 4" },
        { 0, 5, "Game 5" },
         { 0, 6, "Game 6" },
        { 0, 7, "Game 7" },
        { 0, 8, "Game 8" }
    };

    const SDL_MessageBoxData data = {
        SDL_MESSAGEBOX_INFORMATION,
        window,
        "Save Game",
        "Choose a Game slot:",
        SDL_arraysize(buttons),
        buttons,
        NULL
    };

    int buttonid = 0;
    SDL_ShowMessageBox(&data, &buttonid);
    return buttonid; // 1-8 or 0 if closed
}

void showMessageDialog(SDL_Window *window, const char *title, const char *message)
{
    const SDL_MessageBoxButtonData buttons[] = {
        { SDL_MESSAGEBOX_BUTTON_RETURNKEY_DEFAULT, 0, "OK" }
    };

    const SDL_MessageBoxData data = {
        SDL_MESSAGEBOX_WARNING,
        window,
        title,
        message,
        SDL_arraysize(buttons),
        buttons,
        NULL
    };

    int buttonid = 0;
    SDL_ShowMessageBox(&data, &buttonid);
}

void showGameMessage(SDL_Window *window, const char *title, const char *message)
{
    // Display a simple message box attached to the existing window
    SDL_ShowSimpleMessageBox(
        SDL_MESSAGEBOX_INFORMATION, // type: info
        title,                      // title
        message,                    // message content
        window                      // parent window
    );
}

//Highlight clicked button
void highlightClickedButton(SDL_Renderer *renderer,Button cButton)
{
    SDL_Color c = (SDL_Color){.r = 50 , .g = 245 , .b = 245 , .a = 255 *0.50};
    SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(renderer,c.r,c.g,c.b,c.a);
    SDL_RenderFillRect(renderer,&cButton.rect);
    SDL_RenderPresent(renderer);
    SDL_Delay(150); //To show the effect

}
void playSoundEffect(Mix_Chunk *sEffect)
{
    if (sEffect == NULL) {
        printf("Failed to load sound effect! SDL_mixer Error: %s\n", Mix_GetError());
        return ;
    }
    Mix_PlayChannel(-1, sEffect, 0);
}