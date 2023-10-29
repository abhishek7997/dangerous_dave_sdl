#include <iostream>
#include <string>
#include <SDL_image.h>

class TileData
{
public:
    TileData()
    {
        if (IMG_Init(IMG_INIT_PNG) < 0)
        {
            std::cout << "Could not initialize SDL_Image: " << SDL_GetError() << std::endl;
        }
    }
    SDL_Surface *LoadTilemap()
    {
        m_tilemap = IMG_Load(m_tilemap_path.c_str());
        if (!m_tilemap)
        {
            std::cout << "Could not load image: " << SDL_GetError() << std::endl;
        }
        return m_tilemap;
    }
    ~TileData()
    {
        IMG_Quit();
        SDL_FreeSurface(m_tilemap);
    }

private:
    const std::string m_tilemap_path = "D:\\Dev\\GameProgramming\\dangerous_dave_sdl\\assets\\tilemap.png";
    SDL_Surface *m_tilemap;
};