#include "LevelManager.hpp"

LevelManager::LevelManager()
{
    this->Initialize();
}

LevelManager &LevelManager::Get()
{
    static LevelManager instance;
    return instance;
}

Level *LevelManager::GetLevel(const unsigned int &levelIndex)
{
    if (levelIndex > 10)
    {
        return nullptr;
    }
    return m_Levels[levelIndex].get();
}

void LevelManager::RenderLevel(const unsigned int &levelIndex)
{
    if (levelIndex >= m_Levels.size())
        return;

    m_Levels[levelIndex]->RenderLevel();
}

void LevelManager::Initialize()
{
    const int tileWidth = 16;
    const int tileHeight = 16;
    for (int i = 0; i < m_Levels.size(); i++)
    {
        m_Levels[i] = std::make_unique<Level>();
    }

    this->LoadLevels();

    int level = GameState::Get().getCurrentLevel();
    int x = this->GetCurrentLevel()->GetPlayerStartX();
    int y = this->GetCurrentLevel()->GetPlayerStartY();
    GameState::Get().SetPlayerPos(x * 16, y * 16);
}

void LevelManager::LoadLevels()
{
    std::ifstream file(m_LevelsFilePath);
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

void LevelManager::NextLevel()
{
    GameState::Get().Reset();
    GameState::Get().NextLevel();

    const int idx = GameState::Get().getCurrentLevel();

    const int x = m_Levels[idx]->GetPlayerStartX();
    const int y = m_Levels[idx]->GetPlayerStartY();

    GameState::Get().GetPlayer()->SetPlayerPos(x * 16, y * 16);
}

void LevelManager::ResetPlayerPos()
{
    Level *level = this->GetCurrentLevel();
    const int x = level->GetPlayerStartX();
    const int y = level->GetPlayerStartY();
    GameState::Get().GetPlayer()->SetPlayerPos(x * 16, y * 16);
}

void LevelManager::ResetOffset()
{
    this->GetCurrentLevel()->SetOffset(0);
}

Level *LevelManager::GetCurrentLevel()
{
    return this->m_Levels[GameState::Get().getCurrentLevel()].get();
}

int LevelManager::GetCurrentOffset()
{
    return this->m_Levels[GameState::Get().getCurrentLevel()]->GetOffset();
}

LevelManager::~LevelManager()
{
}