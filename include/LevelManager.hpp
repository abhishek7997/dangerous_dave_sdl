#pragma once

#include <array>
#include <SDL.h>
#include "TileManager.hpp"
#include "Level.hpp"
#include "GameObject.hpp"
#include "../src/tile_types.cpp"

using std::vector;

class LevelManager
{
public:
    LevelManager() = delete;
    LevelManager(TileManager *tileManager)
    {
        this->tileManager = tileManager;
        for (int i = 0; i < m_Levels.size(); i++)
        {
            m_Levels[i] = nullptr;
        }
    }

    void Initialize()
    {
        const int tileWidth = 16;
        const int tileHeight = 16;
        for (int i = 0; i < m_Levels.size(); i++)
        {
            m_Levels[i]->CreateLevel();
        }
    }

    void RenderLevel(const int &levelIndex)
    {
        if (levelIndex < 0 || levelIndex >= m_Levels.size())
            return;

        m_Levels[levelIndex]->RenderLevel();
    }

private:
    std::array<Level *, 10> m_Levels;
    TileManager const *tileManager = nullptr;
};