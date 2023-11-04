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
        for (int j = 0; j < 20; j++)
        {
            int height = 3;
            for (int i = 10 - height; i < 10; i++)
            {
                m_Level[i][j] = new GameObject(j * tileHeight, i * tileWidth, StaticObject::WALL_RED, tileManager);
            }
        }

        std::cout << "Static Game objects pushed" << std::endl;

        this->enemyObjects.push_back(new MonsterObject(10, 10, EnemyObject::SPIDER_1, tileManager));
        // this->enemyObjects.push_back(new MonsterObject(10, 10, EnemyObject::UFO_1, tileManager));
        // this->enemyObjects[1]->SetMovements({{16, 7}, {10, 8}, {11, 9}, {12, 10}, {13, 13}, {14, 14}, {14, 11}, {12, 9}, {10, 8}, {16, 7}});
        std::cout << "Monster objects pushed" << std::endl;
    }

    void RenderLevel()
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

        for (MonsterObject *const &obj : this->enemyObjects)
        {
            obj->Move();
            obj->IsColliding(tileManager, m_Level);
            obj->Render(renderer, tileManager);
        }

        // player->Render(renderer, tileManager);
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

    std::array<std::array<GameObject *, 20>, 10> &GetLevel()
    {
        return (this->m_Level);
    }

private:
    const int m_LevelWidth = 20;
    const int m_LevelHeight = 10;
    std::array<std::array<GameObject *, 20>, 10> m_Level;
    SDL_Renderer *renderer = nullptr;
    TileManager *tileManager = nullptr;
    std::vector<MonsterObject *> enemyObjects;
    Player *player = nullptr;
};