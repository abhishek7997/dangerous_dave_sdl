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
        m_LevelData = vector<vector<int>>(m_LevelHeight, vector<int>(m_LevelWidth, 0));
        for (int i = 0; i < m_LevelHeight; i++)
        {
            for (int j = 0; j < m_LevelWidth; j++)
            {
                m_LevelData[i][j] = rand() % 48;
            }
        }

        for (int i = 0; i < m_LevelHeight; i++)
        {
            for (int j = 0; j < m_LevelWidth; j++)
            {
                m_Level[i][j].x = i * 18;
                m_Level[i][j].y = j * 18;
                m_Level[i][j].w = 18;
                m_Level[i][j].h = 18;
            }
        }
    }

    void RenderLevelData(SDL_Renderer *renderer, TileData *const tileData)
    {
        for (int i = 0; i < m_LevelHeight; i++)
        {
            for (int j = 0; j < m_LevelWidth; j++)
            {
                SDL_RenderCopy(renderer, tileData->GetTilemapTexture(), tileData->GetTileById(m_LevelData[i][j]), &m_Level[i][j]);
            }
        }
    }

private:
    const int m_LevelWidth = 35;  // 468/18
    const int m_LevelHeight = 26; // 630/18
    vector<vector<int>> m_LevelData;
    SDL_Rect m_Level[35][26];
};