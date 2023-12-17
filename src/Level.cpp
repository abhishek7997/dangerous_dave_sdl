#include "Level.hpp"

Level::Level()
{
    const int tileWidth = 16;
    const int tileHeight = 16;
    for (int i = 0; i < m_LevelHeight; i++)
    {
        for (int j = 0; j < m_LevelWidth; j++)
        {
            m_Level[i][j] = std::unique_ptr<GameObject>(new GameObject(i * tileHeight, j * tileWidth, StaticObject::EMPTY));
        }
    }
}
void Level::CreateLevel(const std::vector<TileData> &staticTiles, const std::vector<TileData> &monsterObjects, const TileData &player, const std::vector<std::pair<int, int>> &movements)
{
    int tileWidth = 16;
    int tileHeight = 16;

    int i, j;
    for (const TileData &tile : staticTiles)
    {
        i = tile.y;
        j = tile.x;
        m_Level[i][j] = std::unique_ptr<GameObject>(new GameObject(j * tileWidth, i * tileHeight, tile.tileId));
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
                m_Level[i][j]->Render(this->offset);
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
            (*itr)->Render(offset);
            ++itr;
        }
    }
}

SDL_Rect *Level::QueryCell(const int &x, const int &y)
{
    if (x < 0 || x > m_LevelWidth || y < 0 || y > m_LevelHeight)
        return nullptr;
    return &m_Level[x][y]->GetRectangle();
}

void Level::SetPlayerStartingPosition(const int &x, const int &y)
{
    GameState::Get().GetPlayer().get()->SetRectPosition(x, y);
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
    if (x < 0 || x > 9 || y < 0 || y > 99)
    {
        return;
    }
    std::cout << "Clearing cell on " << x << ' ' << y << std::endl;
    // this->m_Level[x][y]->SetTileId(StaticObject::EMPTY);
    m_Level[x][y].reset();
}

int Level::GetPlayerStartX()
{
    return this->playerStartX;
}

int Level::GetPlayerStartY()
{
    return this->playerStartY;
}

int Level::GetTileId(const int &x, const int &y)
{
    if (this->m_Level[x][y] == nullptr)
        return 0;
    return this->m_Level[x][y]->GetTileId();
}