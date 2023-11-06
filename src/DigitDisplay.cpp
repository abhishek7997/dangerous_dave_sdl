#include "DigitDisplay.hpp"

DigitDisplay::DigitDisplay(SDL_Renderer *renderer)
{
    this->renderer = renderer;
    this->tileManager = TileManager::getInstance();
}
void DigitDisplay::RenderText(int x, int num)
{
    if (num > 99999)
        num = 99999;
    std::string str = std::to_string(num);
    SDL_Texture *texture;
    int offset = x;

    for (int i = 0; i < str.size(); i++)
    {
        char c = str[i];
        texture = GetDigitTexture((int)(c)-48);
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

        SDL_Rect dst = {x + i * 8, 0, digitWidth, digitHeight};

        SDL_RenderCopy(renderer, texture, NULL, &dst);
    }
}

SDL_Texture *DigitDisplay::GetDigitTexture(const int &digit)
{
    if (digit < 0 || digit > 9)
        return nullptr;
    SDL_Texture *digitTexture = TileManager::getInstance()->GetTileById(148 + digit);
    return digitTexture;
}