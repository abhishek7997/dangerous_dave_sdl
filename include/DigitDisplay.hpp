#pragma once
#include <string>
#include <SDL.h>
#include "TileManager.hpp"

class DigitDisplay
{
public:
    DigitDisplay(SDL_Renderer *renderer);
    void RenderText(int x, int num);
    SDL_Texture *GetDigitTexture(const int &digit);

private:
    TileManager *tileManager;
    SDL_Renderer *renderer;
    const int digitWidth = 8;
    const int digitHeight = 11;
};