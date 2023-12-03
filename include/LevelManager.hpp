#pragma once

#include <array>
#include <SDL.h>
#include "SDLApp.hpp"
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
    Level *getLevel(const int &index);
    void NextLevel();
    void ResetPlayerPos();
    void ResetOffset();
    Level *GetCurrentLevel();
    int GetCurrentOffset();
    void LoadLevels();

protected:
    static LevelManager *instance;
    LevelManager();
    LevelManager(const LevelManager &) = delete;

private:
    std::array<Level *, 10> m_Levels;
    const SDL_Renderer *renderer = nullptr;
};