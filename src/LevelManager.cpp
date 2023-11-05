#include <SDL.h>
#include "LevelManager.hpp"

void LevelManager::RenderLevel(const int &levelIndex)
{
    if (levelIndex < 0 || levelIndex >= m_Levels.size())
        return;

    m_Levels[levelIndex]->RenderLevel();
}

void LevelManager::Initialize()
{
    if (!renderer)
    {
        SDL_Log("Renderer not set in LevelManager");
        return;
    }
    const int tileWidth = 16;
    const int tileHeight = 16;
    for (int i = 0; i < m_Levels.size(); i++)
    {
        m_Levels[i] = new Level(this->renderer);
        m_Levels[i]->CreateLevel();
    }
    std::cout << "Created :" << m_Levels.size() << " levels" << std::endl;
}

LevelManager *LevelManager::getInstance()
{
    if (instance == nullptr)
    {
        instance = new LevelManager();
    }
    return instance;
}

void LevelManager::SetRenderer(SDL_Renderer *renderer)
{
    this->renderer = renderer;
}

Level *LevelManager::getLevel(const int &index)
{
    if (index < 0 || index > 10)
    {
        return nullptr;
    }
    return m_Levels[index];
}

LevelManager::LevelManager()
{
    for (int i = 0; i < m_Levels.size(); i++)
    {
        m_Levels[i] = nullptr;
    }
    // Initialize();
}

LevelManager *LevelManager::instance = nullptr;