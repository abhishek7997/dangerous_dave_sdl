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
    Level(SDL_Renderer *renderer);
    void CreateLevel();
    void RenderLevel();
    SDL_Rect *QueryCell(const int &x, const int &y);
    void SetPlayerStatringPosition(int x, int y, GameState *&gameState);
    std::array<std::array<GameObject *, 100>, 10> &GetLevel();
    int GetOffset();
    void SetOffset(int o);

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