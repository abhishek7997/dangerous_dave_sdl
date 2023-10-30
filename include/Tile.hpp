#pragma once

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
        m_Tilemap = IMG_Load(m_TilemapPath.c_str());
    }
    TileData(SDL_Renderer *renderer)
    {
        if (IMG_Init(IMG_INIT_PNG) < 0)
        {
            std::cout << "Could not initialize SDL_Image: " << SDL_GetError() << std::endl;
        }
        m_Tilemap = IMG_Load(m_TilemapPath.c_str());
        if (!m_Tilemap)
        {
            std::cout << "Could not load image: " << SDL_GetError() << std::endl;
        }
        m_TilemapTexture = SDL_CreateTextureFromSurface(renderer, m_Tilemap);
    }
    SDL_Texture *GetTilemapTexture()
    {
        if (!m_Tilemap)
        {
            std::cout << "Tilemap not loaded" << std::endl;
            return nullptr;
        }
        return m_TilemapTexture;
    }
    void LoadTiles()
    {
        if (!m_Tilemap)
        {
            std::cout << "Tilemap not loaded" << std::endl;
            return;
        }

        for (int i = 0; i < 8; i++)
        {
            for (int j = 0; j < 6; j++)
            {
                tiles[i][j].x = i * m_TileWidth + m_LineWidth;
                tiles[i][j].y = j * m_TileHeight + m_LineWidth;
                tiles[i][j].w = m_TileWidth - m_LineWidth;
                tiles[i][j].h = m_TileHeight - m_LineWidth;
            }
        }
    }
    SDL_Rect *GetTileById(const int id)
    {
        return &tiles[id / 6][id % 6];
    }
    const int GetTileWidth()
    {
        return m_TileWidth;
    }

    const int GetTileHeight()
    {
        return m_TileHeight;
    }
    ~TileData()
    {
        SDL_FreeSurface(m_Tilemap);
    }

private:
    const std::string m_TilemapPath = "D:\\Dev\\GameProgramming\\dangerous_dave_sdl\\assets\\tilemap_grid.png";
    SDL_Surface *m_Tilemap;
    SDL_Texture *m_TilemapTexture;
    const int m_LineWidth = 2;
    const int m_TileHeight = 18;
    const int m_TileWidth = 18;
    const int m_TilemapWidth = 145;
    const int m_TilemapHeight = 110;
    SDL_Rect tiles[8][6];
};