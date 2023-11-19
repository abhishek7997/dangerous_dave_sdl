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

void LevelManager::NextLevel()
{
    GameState *gameState = GameState::getInstance();
    gameState->Reset();
    gameState->NextLevel();

    const int idx = gameState->getCurrentLevel();

    const int x = m_Levels[idx]->GetPlayerStartX();
    const int y = m_Levels[idx]->GetPlayerStartY();

    gameState->GetPlayer()->SetPlayerPos(x * 16, y * 16);
}

void LevelManager::ResetPlayerPos()
{
    GameState *gameState = GameState::getInstance();
    int lvl = gameState->getCurrentLevel();
    Level *level = this->getLevel(lvl);
    const int x = level->GetPlayerStartX();
    const int y = level->GetPlayerStartY();
    gameState->GetPlayer()->SetPlayerPos(x, y);
}

void LevelManager::ResetOffset()
{
    int lvl = GameState::getInstance()->getCurrentLevel();
    this->getLevel(lvl)->SetOffset(0);
}

Level *LevelManager::GetCurrentLevel()
{
    return this->getLevel(GameState::getInstance()->getCurrentLevel());
}

int LevelManager::GetCurrentOffset()
{
    return this->getLevel(GameState::getInstance()->getCurrentLevel())->GetOffset();
}

LevelManager *LevelManager::instance = nullptr;