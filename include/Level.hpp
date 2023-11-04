#pragma once

#include <SDL.h>
#include <vector>
#include <array>
#include "GameObject.hpp"
#include "TileManager.hpp"
#include "GameState.hpp"
#include "../src/tile_types.cpp"

class Level
{
public:
    Level(SDL_Renderer *renderer, TileManager *&tileManager)
    {
        this->renderer = renderer;
        this->tileManager = tileManager;

        this->player = new Player();

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
        for (int j = 0; j < m_LevelWidth; j++)
        {
            int height = 3;
            for (int i = 10 - height; i < m_LevelHeight; i++)
            {
                m_Level[i][j] = new GameObject(j * tileHeight, i * tileWidth, StaticObject::WALL_RED, tileManager);
            }
        }

        m_Level[4][21] = new GameObject(21 * 16, 4 * 16, StaticObject::WALL_BLUE, tileManager);
        m_Level[4][39] = new GameObject(39 * 16, 4 * 16, StaticObject::WALL_BLUE, tileManager);
        m_Level[3][40] = new GameObject(40 * 16, 3 * 16, StaticObject::FIRE_1, tileManager);
        m_Level[3][45] = new GameObject(45 * 16, 3 * 16, StaticObject::FIRE_2, tileManager);
        m_Level[4][65] = new GameObject(65 * 16, 4 * 16, StaticObject::TROPHY_1, tileManager);
        m_Level[3][75] = new GameObject(75 * 16, 3 * 16, StaticObject::JETPACK, tileManager);
        m_Level[5][95] = new GameObject(95 * 16, 5 * 16, StaticObject::GUN, tileManager);

        std::cout
            << "Static Game objects pushed" << std::endl;

        this->enemyObjects.push_back(new MonsterObject(10, 10, EnemyObject::SPIDER_1, tileManager));
        // this->enemyObjects.push_back(new MonsterObject(10, 10, EnemyObject::UFO_1, tileManager));
        // this->enemyObjects[1]->SetMovements({{16, 7}, {10, 8}, {11, 9}, {12, 10}, {13, 13}, {14, 14}, {14, 11}, {12, 9}, {10, 8}, {16, 7}});
        std::cout << "Monster objects pushed" << std::endl;
    }

    void RenderLevel()
    {
        int tileWidth = 16;
        int tileHeight = 16;
        for (int i = 0; i < m_LevelHeight; i++)
        {
            for (int j = 0; j < m_LevelWidth; j++)
            {
                // std::cout << "I: " << i << "J: " << j << std::endl;
                if (m_Level[i][j])
                    m_Level[i][j]->Render(renderer, tileManager, offset);
            }
        }

        for (MonsterObject *const &obj : this->enemyObjects)
        {
            obj->Move();
            obj->IsColliding(tileManager, m_Level);
            obj->Render(renderer, tileManager, offset);
        }
    }

    SDL_Rect *QueryCell(const int &x, const int &y)
    {
        if (x < 0 || x > m_LevelHeight || y < 0 || y > m_LevelHeight)
            return nullptr;
        return m_Level[x][y]->GetRectangle();
    }

    void SetPlayerStatringPosition(int x, int y, GameState *&gameState)
    {
        gameState->SetPlayer(player);
    }

    std::array<std::array<GameObject *, 100>, 10> &GetLevel()
    {
        return (this->m_Level);
    }

    int GetOffset()
    {
        return this->offset;
    }

    void SetOffset(int o)
    {
        this->offset = o;
    }

private:
    const int m_LevelWidth = 100;
    const int m_LevelHeight = 10;
    int offset = 0;
    std::array<std::array<GameObject *, 100>, 10> m_Level;
    SDL_Renderer *renderer = nullptr;
    TileManager *tileManager = nullptr;
    std::vector<MonsterObject *> enemyObjects;
    Player *player = nullptr;
};