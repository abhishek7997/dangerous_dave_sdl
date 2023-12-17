#include "DigitDisplay.hpp"

DigitDisplay::DigitDisplay()
{
}

void DigitDisplay::RenderText(const int &x, int num)
{
    if (num > 99999)
    {
        num = 99999;
    }

    std::shared_ptr<SDL_Renderer> renderer = SDLApp::Get().GetRenderer();
    std::string str = std::to_string(num);
    std::shared_ptr<SDL_Texture> texture;
    SDL_Rect dst;

    for (int i = 0; i < str.size(); i++)
    {
        char c = str[i];
        texture = GetDigitTexture((int)(c)-48);
        SDL_SetTextureBlendMode(texture.get(), SDL_BLENDMODE_BLEND);

        dst = {x + i * this->digitWidth, 0, digitWidth, digitHeight};

        SDL_RenderCopy(renderer.get(), texture.get(), NULL, &dst);
    }
}

std::shared_ptr<SDL_Texture> DigitDisplay::GetDigitTexture(unsigned int digit)
{
    if (digit > 9)
    {
        digit = 9;
    }
    return TileManager::Get().GetTileById(MiscObject::DIGIT_ZERO + digit);
}

DigitDisplay::~DigitDisplay()
{
    std::cout << "DigitDisplay Destructor called" << std::endl;
}