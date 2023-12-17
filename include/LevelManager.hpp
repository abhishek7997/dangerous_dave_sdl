#pragma once

#include <array>
#include <SDL.h>
#include "TileManager.hpp"
#include "Level.hpp"
#include "tile_types.hpp"

class LevelManager
{
public:
    static LevelManager &Get();
    Level *GetLevel(const unsigned int &levelIndex);
    Level *GetCurrentLevel();
    LevelManager(const LevelManager &) = delete;
    void operator=(const LevelManager &) = delete;
    void RenderLevel(const unsigned int &levelIndex);
    void Initialize();
    void NextLevel();
    void ResetPlayerPos();
    void ResetOffset();
    void LoadLevels();
    int GetCurrentOffset();
    ~LevelManager();

private:
    LevelManager();
    const std::string m_LevelsFilePath = "D:\\Dev\\GameProgramming\\dangerous_dave_sdl\\assets\\levels.txt";
    static LevelManager instance;
    std::array<std::unique_ptr<Level>, 10> m_Levels;
};