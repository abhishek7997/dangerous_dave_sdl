#pragma once

#include <SDL.h>
#include <vector>
#include "GameObject.hpp"
#include "TileManager.hpp"
#include "../src/tile_types.cpp"

class Level
{
public:
    Level(SDL_Renderer *renderer, TileManager *&tileManager)
    {
        this->renderer = renderer;
        this->tileManager = tileManager;

        const int tileWidth = 16;
        const int tileHeight = 16;
        for (int i = 0; i < m_LevelHeight; i++)
        {
            for (int j = 0; j < m_LevelWidth; j++)
            {
                m_Level[i][j] = new GameObject(i * tileHeight, j * tileWidth, StaticObject::EMPTY, this->tileManager);
            }
        }
    }
    void CreateLevel()
    {
        if (!this->tileManager)
        {
            SDL_Log("TileManager not loaded in CreateLevel()\n");
            return;
        }
        // static objects (walls)
        int tileWidth = 16;
        int tileHeight = 16;
        for (int i = 0; i < 2; i++)
        {
            for (int j = 0; j < 2; j++)
            {
                if (i == 1 && j == 1)
                    continue;
                m_Level[i][j] = new GameObject(i * tileHeight, j * tileWidth, StaticObject::WALL_RED, tileManager);
            }
        }

        std::cout << "Static Game objects pushed" << std::endl;
        // monster object
        this->enemyObjects.push_back(new MonsterObject(10, 10, EnemyObject::SPIDER_1, tileManager));
        // this->enemyObjects.push_back(new MonsterObject(10, 10, EnemyObject::UFO_1, tileManager));
        // this->enemyObjects[1]->SetMovements({{16, 7}, {10, 8}, {11, 9}, {12, 10}, {13, 13}, {14, 14}, {14, 11}, {12, 9}, {10, 8}, {16, 7}});
        std::cout << "Monster objects pushed" << std::endl;
    }

    void RenderLevel(TileManager *const tileManager)
    {
        int tileWidth = 16;
        int tileHeight = 16;
        for (int i = 0; i < 10; i++)
        {
            for (int j = 0; j < 20; j++)
            {
                if (m_Level[i][j])
                    m_Level[i][j]->Render(renderer, tileManager);
            }
        }

        for (MonsterObject *obj : this->enemyObjects)
        {
            obj->Move();
            obj->IsColliding(tileManager, &m_Level);
            obj->Render(renderer, tileManager);
        }
    }

    SDL_Rect *QueryCell(const int x, const int y)
    {
        if (x < 0 || x > m_LevelHeight || y < 0 || y > m_LevelHeight)
            return nullptr;
        return m_Level[x][y]->GetRectangle();
    }

private:
    const int m_LevelWidth = 20;
    const int m_LevelHeight = 10;
    GameObject *m_Level[10][20];
    SDL_Renderer *renderer;
    TileManager *tileManager;
    std::vector<MonsterObject *> enemyObjects;
};