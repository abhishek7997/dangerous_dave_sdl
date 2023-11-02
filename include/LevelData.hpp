#pragma once

#include <cstdlib>
#include <ctime>
#include <vector>
#include <SDL.h>
#include "TileManager.hpp"
#include "../src/tile_types.cpp"

using std::vector;

class LevelData
{
public:
    LevelData()
    {
        srand(time(NULL));
        m_LevelData = vector<vector<int>>(m_LevelHeight, vector<int>(m_LevelWidth, 0));
        // for (int i = 0; i < m_LevelHeight; i++)
        // {
        //     for (int j = 0; j < m_LevelWidth; j++)
        //     {
        //         m_LevelData[i][j] = 0;
        //     }
        // }
        for (int j = 0; j < m_LevelWidth; j++)
        {
            int height = 3 + rand() % (7 - 3 + 1);
            for (int i = height; i < m_LevelHeight; i++)
            {
                m_LevelData[i][j] = StaticObject::WALL_RED;
            }
        }

        for (int i = 0; i < m_LevelHeight; i++)
        {
            for (int j = 0; j < m_LevelWidth; j++)
            {
                m_Level[i][j].x = j * m_TileWidth;
                m_Level[i][j].y = i * m_TileHeight;
                m_Level[i][j].w = m_TileWidth;
                m_Level[i][j].h = m_TileHeight;
            }
        }
    }

    void RenderLevelData(SDL_Renderer *renderer, TileManager *const tileData)
    {
        for (int i = 0; i < m_LevelHeight; i++)
        {
            for (int j = 0; j < m_LevelWidth; j++)
            {
                SDL_Texture *texture = tileData->GetTileById(m_LevelData[i][j]);
                int w, h;
                if (SDL_QueryTexture(texture, NULL, NULL, &w, &h) < 0)
                {
                    SDL_Log("Failed to query texture: %s", SDL_GetError());
                    continue;
                }

                m_Level[i][j].h = h;
                m_Level[i][j].w = w;
                SDL_RenderCopy(renderer, texture, NULL, &m_Level[i][j]);
            }
        }
    }

    void SetLevelDataTile(const int &x, const int &y, int &id)
    {
        m_LevelData[x][y] = id;
    }

    const int getLevelWidth()
    {
        return m_LevelWidth;
    }

    const int getLevelHeight()
    {
        return m_LevelHeight;
    }

private:
    const int m_LevelWidth = 20;  // 468/18
    const int m_LevelHeight = 10; // 630/18
    const int m_TileWidth = 16;
    const int m_TileHeight = 16;
    // const int m_LevelWidth = 26;  // 468/18
    // const int m_LevelHeight = 35; // 630/18
    vector<vector<int>> m_LevelData;
    SDL_Rect m_Level[10][20];
};