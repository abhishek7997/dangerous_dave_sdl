#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <tuple>
#include <list>
#include <array>
#include <SDL.h>
#include "SDLApp.hpp"
#include "GameObject.hpp"
#include "TileManager.hpp"
#include "GameState.hpp"
#include "tile_types.hpp"

class Level
{
public:
    Level();
    void CreateLevel(const std::vector<TileData> &staticTiles, const std::vector<TileData> &monsterObjects, const TileData &player, const std::vector<std::pair<int, int>> &movements);
    void RenderLevel();
    SDL_Rect *QueryCell(const int &x, const int &y);
    void SetPlayerStartingPosition(const int &x, const int &y);
    int GetOffset();
    void SetOffset(const int &o);
    void ClearCell(const int &x, const int &y);
    int GetPlayerStartX();
    int GetPlayerStartY();
    int GetTileId(const int &x, const int &y);

private:
    const int m_LevelWidth = 100;
    const int m_LevelHeight = 10;
    int offset = 0;
    int playerStartX = 10;
    int playerStartY = 10;
    std::array<std::array<std::unique_ptr<GameObject>, 100>, 10> m_Level;
    std::list<MonsterObject *> enemyObjects;
};