#pragma once

#include <cstdlib>
#include <ctime>
#include <vector>
#include <SDL.h>
#include "Tile.hpp"

using std::vector;

class LevelData
{
public:
    LevelData()
    {
        srand(time(NULL));
        m_LevelData = vector<vector<int>>(m_LevelWidth, vector<int>(m_LevelHeight, 0));
        for (int i = 0; i < m_LevelWidth; i++)
        {
            for (int j = 0; j < m_LevelHeight; j++)
            {
                m_LevelData[i][j] = rand() % 125;
            }
        }

        for (int i = 0; i < m_LevelWidth; i++)
        {
            for (int j = 0; j < m_LevelHeight; j++)
            {
                m_Level[i][j].x = i * m_TileWidth;
                m_Level[i][j].y = j * m_TileHeight;
                m_Level[i][j].w = m_TileWidth;
                m_Level[i][j].h = m_TileHeight;
            }
        }
    }

    void RenderLevelData(SDL_Renderer *renderer, TileManager *const tileData)
    {
        for (int i = 0; i < m_LevelWidth; i++)
        {
            for (int j = 0; j < m_LevelHeight; j++)
            {
                SDL_Texture *texture = tileData->GetTileById(m_LevelData[i][j]);
                int w, h;
                if (SDL_QueryTexture(texture, NULL, NULL, &w, &h) < 0)
                {
                    SDL_Log("Failed to query texture: %s", SDL_GetError());
                }
                else
                {
                    m_Level[i][j].h = h;
                    m_Level[i][j].w = w;
                }
                SDL_RenderCopy(renderer, texture, NULL, &m_Level[i][j]);
            }
        }
    }

    void SetLevelDataTile(const int x, const int y, int id)
    {
        m_LevelData[x][y] = id;
    }

private:
    const int m_LevelWidth = 35;  // 468/18
    const int m_LevelHeight = 26; // 630/18
    const int m_TileWidth = 16;
    const int m_TileHeight = 16;
    // const int m_LevelWidth = 26;  // 468/18
    // const int m_LevelHeight = 35; // 630/18
    vector<vector<int>> m_LevelData;
    SDL_Rect m_Level[35][26];
};