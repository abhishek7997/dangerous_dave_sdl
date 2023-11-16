#include "Level.hpp"

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
void Level::CreateLevel()
{
    if (!this->tileManager)
    {
        SDL_Log("TileManager not loaded in CreateLevel()\n");
        return;
    }
    // static objects (walls)
    int tileWidth = 16;
    int tileHeight = 16;
    for (int j = 0; j < m_LevelWidth; j++)
    {
        int height = 3;
        for (int i = 10 - height; i < m_LevelHeight; i++)
        {
            m_Level[i][j] = new GameObject(j * tileHeight, i * tileWidth, StaticObject::WALL_RED);
        }
    }

    m_Level[5][15] = new GameObject(15 * 16, 5 * 16, StaticObject::WALL_BLUE);
    m_Level[4][21] = new GameObject(21 * 16, 4 * 16, StaticObject::WALL_BLUE);
    m_Level[4][39] = new GameObject(39 * 16, 4 * 16, StaticObject::WALL_BLUE);
    m_Level[3][40] = new GameObject(40 * 16, 3 * 16, StaticObject::POINT_BLUE);
    m_Level[3][45] = new GameObject(45 * 16, 3 * 16, StaticObject::POINT_RED);
    m_Level[4][45] = new GameObject(45 * 16, 4 * 16, StaticObject::FIRE_1);
    m_Level[4][46] = new GameObject(46 * 16, 4 * 16, StaticObject::FIRE_1);
    m_Level[4][47] = new GameObject(47 * 16, 4 * 16, StaticObject::FIRE_1);
    m_Level[3][55] = new GameObject(55 * 16, 3 * 16, StaticObject::WATER_1);
    m_Level[3][56] = new GameObject(56 * 16, 3 * 16, StaticObject::WATER_1);
    m_Level[3][57] = new GameObject(57 * 16, 3 * 16, StaticObject::WATER_1);
    m_Level[4][65] = new GameObject(65 * 16, 4 * 16, StaticObject::TROPHY_1);
    m_Level[7][75] = new GameObject(75 * 16, 7 * 16, StaticObject::JETPACK);
    m_Level[6][95] = new GameObject(95 * 16, 6 * 16, StaticObject::GUN);
    m_Level[6][98] = new GameObject(98 * 16, 6 * 16, StaticObject::DOOR);

    this->enemyObjects.push_back(new MonsterObject(10, 10, EnemyObject::SPIDER_1));
    this->enemyObjects.push_back(new MonsterObject(60 * 16, 10, EnemyObject::SPIDER_1));
    // this->enemyObjects.push_back(new MonsterObject(25 * 16, 5 * 16, EnemyObject::UFO_1));
    // this->enemyObjects[1]->SetMovements({{16, 7}, {10, 8}, {11, 9}, {12, 10}, {13, 13}, {14, 14}, {14, 11}, {12, 9}, {10, 8}, {16, 7}});
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