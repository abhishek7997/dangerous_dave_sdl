#pragma once

#include <array>
#include <SDL.h>
#include "Level.hpp"
#include "TileManager.hpp"
#include "../src/tile_types.cpp"

class LevelManager
{
public:
    void RenderLevel(const int &levelIndex);
    LevelManager(LevelManager &other) = delete;
    void operator=(const LevelManager &) = delete;
    void Initialize();
    static LevelManager *getInstance();
    void SetRenderer(SDL_Renderer *renderer);
    Level *getLevel(const int &index);

protected:
    static LevelManager *instance;
    LevelManager();

private:
    std::array<Level *, 10> m_Levels;
    SDL_Renderer *renderer = nullptr;
};