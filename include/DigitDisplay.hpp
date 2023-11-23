#pragma once
#include <string>
#include <SDL.h>
#include "tile_types.hpp"
#include "TileManager.hpp"

class DigitDisplay
{
public:
    DigitDisplay(SDL_Renderer *renderer);
    void RenderText(int x, int num);
    SDL_Texture *GetDigitTexture(const int &digit);

private:
    SDL_Renderer *renderer;
    const int digitWidth = 8;
    const int digitHeight = 11;
};