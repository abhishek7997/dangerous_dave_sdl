#pragma once

#include <array>
#include <SDL.h>
#include "Level.hpp"
#include "TileManager.hpp"
#include "tile_types.hpp"

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
    void NextLevel();
    void ResetPlayerPos();
    void ResetOffset();
    Level *GetCurrentLevel();
    int GetCurrentOffset();

protected:
    static LevelManager *instance;
    LevelManager();

private:
    std::array<Level *, 10> m_Levels;
    SDL_Renderer *renderer = nullptr;
};