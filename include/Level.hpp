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
                m_LevelData[i][j] = rand() % 48;
            }
        }

        for (int i = 0; i < m_LevelWidth; i++)
        {
            for (int j = 0; j < m_LevelHeight; j++)
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
        for (int i = 0; i < m_LevelWidth; i++)
        {
            for (int j = 0; j < m_LevelHeight; j++)
            {
                SDL_RenderCopy(renderer, tileData->GetTilemapTexture(), tileData->GetTileById(m_LevelData[i][j]), &m_Level[i][j]);
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
    // const int m_LevelWidth = 26;  // 468/18
    // const int m_LevelHeight = 35; // 630/18
    vector<vector<int>> m_LevelData;
    SDL_Rect m_Level[35][26];
};