#include "Level.hpp"

std::vector<TileData> levelTiles;

Level::Level()
{
    this->renderer = SDLApp::getInstance()->GetRenderer();
    this->tileManager = TileManager::getInstance();

    const int tileWidth = 16;
    const int tileHeight = 16;
    for (int i = 0; i < m_LevelHeight; i++)
    {
        for (int j = 0; j < m_LevelWidth; j++)
        {
            m_Level[i][j] = new GameObject(i * tileHeight, j * tileWidth, StaticObject::EMPTY);
        }
    }
}
void Level::CreateLevel(const std::vector<TileData> &staticTiles, const std::vector<TileData> &monsterObjects, const TileData player, const std::vector<std::pair<int, int>> &movements)
{
    if (!this->tileManager)
    {
        SDL_Log("TileManager not loaded in CreateLevel()\n");
        return;
    }
    // static objects (walls)
    int tileWidth = 16;
    int tileHeight = 16;

    int i, j;
    for (const TileData &tile : staticTiles)
    {
        i = tile.y;
        j = tile.x;
        m_Level[i][j] = new GameObject(j * tileWidth, i * tileHeight, tile.tileId);
    }

    for (const TileData &tile : monsterObjects)
    {
        i = tile.y;
        j = tile.x;
        MonsterObject *obj = new MonsterObject(j * tileWidth, i * tileHeight, tile.tileId);
        obj->SetMovements(movements);
        this->enemyObjects.push_back(obj);
    }

    this->playerStartX = player.x;
    this->playerStartY = player.y;
}

void Level::RenderLevel()
{
    int tileWidth = 16;
    int tileHeight = 16;
    for (int i = 0; i < m_LevelHeight; i++)
    {
        for (int j = 0; j < m_LevelWidth; j++)
        {
            if (m_Level[i][j])
            {
                m_Level[i][j]->Render(this->renderer, this->offset);
                m_Level[i][j]->UpdateFrame(j);
            }
        }
    }

    std::list<MonsterObject *>::iterator itr = this->enemyObjects.begin();

    for (itr = this->enemyObjects.begin(); itr != this->enemyObjects.end();)
    {
        if ((*itr)->IsColliding())
        {
            delete *itr;
            itr = this->enemyObjects.erase(itr);
        }
        else
        {
            (*itr)->UpdateFrame();
            (*itr)->Render(renderer, offset);
            (*itr)->RenderBullet(renderer, offset);
            ++itr;
        }
    }
}

SDL_Rect *Level::QueryCell(const int &x, const int &y)
{
    if (x < 0 || x > m_LevelWidth || y < 0 || y > m_LevelHeight)
        return nullptr;
    return m_Level[x][y]->GetRectangle();
}

void Level::SetPlayerStartingPosition(const int &x, const int &y)
{
    Player *player = GameState::getInstance()->GetPlayer();
    player->SetRectPosition(x, y);
}

std::array<std::array<GameObject *, 100>, 10> &Level::GetLevel()
{
    return (this->m_Level);
}

int Level::GetOffset()
{
    return this->offset;
}

void Level::SetOffset(const int &o)
{
    this->offset = o;
}

void Level::ClearCell(const int &x, const int &y)
{
    if (x < 0 || x > 10 || y < 0 || y > 99)
        return;
    this->m_Level[x][y]->SetTileId(StaticObject::EMPTY);
}

int Level::GetPlayerStartX()
{
    return this->playerStartX;
}

int Level::GetPlayerStartY()
{
    return this->playerStartY;
}