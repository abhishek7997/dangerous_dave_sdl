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
        //  m_Levels[i]->CreateLevel();
    }
    // std::cout << "Created :" << m_Levels.size() << " levels" << std::endl;
    this->LoadLevels();

    int level = GameState::getInstance()->getCurrentLevel();
    std::cout << "Level no: " << level << std::endl;
    int x = this->GetCurrentLevel()->GetPlayerStartX();
    int y = this->GetCurrentLevel()->GetPlayerStartY();
    std::cout << "X: " << x << ' ' << "Y: " << y << std::endl;
    GameState::getInstance()->SetPlayerPos(x * 16, y * 16);
}

void LevelManager::LoadLevels()
{
    std::ifstream file("D:\\Dev\\GameProgramming\\dangerous_dave_sdl\\assets\\levels.txt");
    if (!file.is_open())
    {
        std::cerr << "Could not read level data" << std::endl;
        std::exit(1);
    }

    int level = 0;

    std::vector<TileData> levelTiles;
    std::vector<TileData> enemyObjects;
    std::vector<std::pair<int, int>> movements;
    TileData player;

    std::string line;
    while (level < 10 && std::getline(file, line))
    {
        if (line.empty() || (line.size() == 1 && line[0] == '/'))
        {
            continue;
        }
        else if (line[0] == '$')
            break;
        else if (line[0] == '@')
        {
            // ++level;
        }
        else if (line.size() == 2 && line[0] == '_' && line[1] == '_')
        {
            m_Levels[level++]->CreateLevel(levelTiles, enemyObjects, player, movements);
            levelTiles.clear();
            enemyObjects.clear();
            std::cout << "Set Level: " << level << std::endl;
            continue;
        }
        else if (line.size() == 2 && line[0] == '/' && line[1] == 'L')
        {
            while (std::getline(file, line))
            {
                if (line[0] == '$' || line[0] == '_')
                    break;
                std::stringstream ss(line);
                int x, y, tileId;
                char comma;
                ss >> x >> comma >> y >> comma >> tileId;
                levelTiles.push_back(TileData(x, y, tileId));
            }
        }
        else if (line.size() == 2 && line[0] == '/' && line[1] == 'M')
        {
            uint8_t monsterCount, i = 0;
            std::getline(file, line);
            std::stringstream ss(line);
            ss >> monsterCount;
            while (i++ < monsterCount && std::getline(file, line))
            {
                if (line[0] == '_')
                    break;
                std::stringstream ss(line);
                int x, y, tileId;
                char comma;
                ss >> x >> comma >> y >> comma >> tileId;
                enemyObjects.push_back(TileData(x, y, tileId));
            }
        }
        else if (line.size() == 2 && line[0] == '/' && line[1] == 'm')
        {
            std::getline(file, line);
            std::stringstream ss(line);
            int dx, dy;
            while (std::getline(file, line))
            {
                if (line.size() >= 1 && line[0] == '_')
                    break;
                std::stringstream ss(line);
                int x, y, tileId;
                char comma;
                ss >> dx >> comma >> dy;
                movements.push_back({dx, dy});
            }
        }
        else if (line.size() == 2 && line[0] == '/' && line[1] == 'P')
        {
            std::getline(file, line);
            std::stringstream ss(line);
            int playerX, playerY;
            char comma;
            ss >> playerX >> comma >> playerY;
            player.x = playerX;
            player.y = playerY;
        }
    }

    this->ResetPlayerPos();
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
    GameState::getInstance()->Reset();
    GameState::getInstance()->NextLevel();

    const int idx = GameState::getInstance()->getCurrentLevel();

    const int x = m_Levels[idx]->GetPlayerStartX();
    const int y = m_Levels[idx]->GetPlayerStartY();

    GameState::getInstance()->GetPlayer()->SetPlayerPos(x * 16, y * 16);
}

void LevelManager::ResetPlayerPos()
{
    Level *level = this->GetCurrentLevel();
    const int x = level->GetPlayerStartX();
    const int y = level->GetPlayerStartY();
    GameState::getInstance()->GetPlayer()->SetPlayerPos(x * 16, y * 16);
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