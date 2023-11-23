#include "Level.hpp"

std::vector<TileData> levelTiles;

Level::Level(SDL_Renderer *renderer)
{
    this->renderer = renderer;
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
void Level::CreateLevel(const std::vector<TileData> staticTiles, const std::vector<TileData> monsterObjects, const TileData player)
{
    if (!this->tileManager)
    {
        SDL_Log("TileManager not loaded in CreateLevel()\n");
        return;
    }
    // static objects (walls)
    int tileWidth = 16;
    int tileHeight = 16;

    // Creating Level 1

    // Border walls
    // for (int i = 0; i < 19; i++)
    // {
    //     levelTiles.push_back({i, 0, StaticObject::WALL_RED});
    //     levelTiles.push_back({i, 9, StaticObject::WALL_RED});
    // }

    // for (int i = 0; i < 10; i++)
    // {
    //     levelTiles.push_back({0, i, StaticObject::WALL_RED});
    //     levelTiles.push_back({18, i, StaticObject::WALL_RED});
    //     levelTiles.push_back({19, i, StaticObject::WALL_MUD});
    // }

    // // Other walls
    // levelTiles.push_back({3, 3, StaticObject::WALL_RED});
    // levelTiles.push_back({7, 3, StaticObject::WALL_RED});
    // levelTiles.push_back({11, 3, StaticObject::WALL_RED});
    // levelTiles.push_back({15, 3, StaticObject::WALL_RED});

    // levelTiles.push_back({1, 5, StaticObject::WALL_RED});
    // levelTiles.push_back({5, 5, StaticObject::WALL_RED});
    // levelTiles.push_back({9, 5, StaticObject::WALL_RED});
    // levelTiles.push_back({13, 5, StaticObject::WALL_RED});
    // levelTiles.push_back({17, 5, StaticObject::WALL_RED});

    // levelTiles.push_back({4, 7, StaticObject::WALL_RED});
    // levelTiles.push_back({5, 7, StaticObject::WALL_RED});
    // levelTiles.push_back({6, 7, StaticObject::WALL_RED});
    // levelTiles.push_back({7, 7, StaticObject::WALL_RED});

    // levelTiles.push_back({11, 7, StaticObject::WALL_RED});
    // levelTiles.push_back({12, 7, StaticObject::WALL_RED});
    // levelTiles.push_back({13, 7, StaticObject::WALL_RED});
    // levelTiles.push_back({14, 7, StaticObject::WALL_RED});
    // levelTiles.push_back({15, 7, StaticObject::WALL_RED});
    // levelTiles.push_back({16, 7, StaticObject::WALL_RED});

    // levelTiles.push_back({11, 8, StaticObject::WALL_RED});

    // // Blue Points
    // levelTiles.push_back({3, 2, StaticObject::POINT_BLUE});
    // levelTiles.push_back({7, 2, StaticObject::POINT_BLUE});
    // levelTiles.push_back({15, 2, StaticObject::POINT_BLUE});

    // levelTiles.push_back({1, 4, StaticObject::POINT_BLUE});
    // levelTiles.push_back({5, 4, StaticObject::POINT_BLUE});
    // levelTiles.push_back({9, 4, StaticObject::POINT_BLUE});
    // levelTiles.push_back({13, 4, StaticObject::POINT_BLUE});
    // levelTiles.push_back({17, 4, StaticObject::POINT_BLUE});

    // levelTiles.push_back({1, 6, StaticObject::POINT_BLUE});
    // levelTiles.push_back({7, 6, StaticObject::POINT_BLUE});

    // // Purple point
    // levelTiles.push_back({1, 1, StaticObject::POINT_PURPLE});

    // // Red point
    // levelTiles.push_back({17, 1, StaticObject::POINT_RED});

    // // Trophy
    // levelTiles.push_back({11, 2, StaticObject::TROPHY_1});

    // // Door
    // levelTiles.push_back({12, 8, StaticObject::DOOR});

    // // Pipe
    // levelTiles.push_back({1, 8, StaticObject::PIPE_RIGHT});

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
        this->enemyObjects.push_back(new MonsterObject(j * tileWidth, i * tileHeight, tile.tileId));
    }

    // Set player starting position
    this->playerStartX = player.x;
    this->playerStartY = player.y;

    // GameState::getInstance()->SetPlayerPos(this->playerStartX * 16, this->playerStartY * 16);
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
    MonsterObject *obj;

    for (itr = this->enemyObjects.begin(); itr != this->enemyObjects.end();)
    {
        obj = *itr;
        obj->Move();
        if (obj->IsColliding())
        {
            this->enemyObjects.erase(itr++);
        }
        else
        {
            obj->Render(renderer, offset);
            ++itr;
        }
    }

    // for (MonsterObject *const &obj : this->enemyObjects)
    // {
    //     obj->Move();
    //     if (obj->IsColliding()) {
    //         obj->
    //     }
    //     obj->Render(renderer, offset);
    // }
}

SDL_Rect *Level::QueryCell(const int &x, const int &y)
{
    if (x < 0 || x > m_LevelWidth || y < 0 || y > m_LevelHeight)
        return nullptr;
    return m_Level[x][y]->GetRectangle();
}

void Level::SetPlayerStartingPosition(const int x, const int y)
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

void Level::SetOffset(int o)
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