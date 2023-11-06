#include <array>
#include "GameState.hpp"

void IGameObject::Render(SDL_Renderer *renderer, const int &offset) const
{
    TileManager *tileManager = TileManager::getInstance();
    if (!tileManager)
    {
        SDL_Log("TileManager not loaded");
        return;
    }
    SDL_Texture *texture = tileManager->GetTileById(tileId);
    if (!texture)
    {
        SDL_Log("Could not query texture :( %s", SDL_GetError());
        return;
    }
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

    SDL_Rect dst = {this->rect.x - offset, this->rect.y + 16, this->rect.w, this->rect.h};

    SDL_RenderCopy(renderer, texture, NULL, &dst);

    if (tileId != 0)
    {
        SDL_SetRenderDrawColor(renderer, 0xff, 0x0, 0xff, 0xff);
        SDL_RenderDrawRect(renderer, &dst);
    }
};

void IGameObject::SetRectPosition(const int &x, const int &y)
{
    (this->rect).x = x;
    (this->rect).y = y;
};

void IGameObject::SetRectDimension(const int &w, const int &h)
{
    (this->rect).w = w;
    (this->rect).h = h;
}

int IGameObject::GetTileId()
{
    return this->tileId;
}

SDL_bool IGameObject::isColliding(const SDL_Rect *other)
{
    return SDL_HasIntersection(&(this->rect), other);
}

SDL_Rect *IGameObject::GetRectangle()
{
    return &(this->rect);
}

void IGameObject::SetTileId(const int &tileId)
{
    if (tileId < 0 || tileId > 157)
    {
        this->tileId = 0;
        return;
    }
    this->tileId = tileId;
}

// GameObject
GameObject::GameObject(const int &x, const int &y, const int &tileId)
{
    int tileWidth, tileHeight;
    TileManager *tileManager = TileManager::getInstance();
    SDL_Texture *texture = tileManager->GetTileById(tileId);
    if (SDL_QueryTexture(texture, NULL, NULL, &tileWidth, &tileHeight) < 0)
    {
        if (!texture)
            std::cout << "Texture is null" << std::endl;
        std::cout << "Could not query texture in GameObject(): " << SDL_GetError() << std::endl;
        return;
    }
    SetRectPosition(x, y);
    SetRectDimension(tileWidth, tileHeight);
    this->tileId = tileId;
}

void GameObject::SetPosition(const int &x, const int &y)
{
    this->rect.x = x;
    this->rect.y = y;
}

// MonsterObject
MonsterObject::MonsterObject(const int &x, const int &y, const int &tileId)
{
    int tileWidth, tileHeight;
    TileManager *tileManager = TileManager::getInstance();
    SDL_Texture *texture = tileManager->GetTileById(tileId);
    if (SDL_QueryTexture(texture, NULL, NULL, &tileWidth, &tileHeight) < 0)
    {
        std::cout << "Could not query texture in MonsterObject(): " << SDL_GetError() << std::endl;
        return;
    }

    SetRectPosition(x, y);
    SetRectDimension(tileWidth, tileHeight);
    SetTileId(tileId);
    this->movements = {{4, 2}, {6, 4}, {8, 6}, {10, 8}, {12, 10}, {12, 12}, {12, 10}, {12, 8}, {12, 6}, {12, 4}, {12, 2}, {10, 2}, {8, 2}, {6, 2}};
    this->size = this->movements.size();
    this->iterator = this->movements.begin();
}

void MonsterObject::Move()
{
    if (this->iterator == this->movements.end())
        this->iterator = this->movements.begin();
    int x = (*(this->iterator)).first * 3;
    int y = (*(this->iterator)).second * 3;
    SetRectPosition(x, y);
    this->iterator++;
}

void MonsterObject::SetMovements(std::vector<std::pair<int, int>> movements)
{
    this->movements = movements;
}

SDL_bool MonsterObject::IsColliding(std::array<std::array<GameObject *, 100>, 10> &level)
{
    TileManager *tileManager = TileManager::getInstance();
    if (!tileManager)
    {
        SDL_Log("Tile manager not set\n");
        return SDL_FALSE;
    }
    int currX = this->rect.x;
    int currY = this->rect.y;
    int cellX = currX / 16;
    int cellY = currY / 16;
    int tileIndex = 0;

    for (int i = cellX; i <= cellX + 1; i++)
    {
        for (int j = cellY; j <= cellY + 1; j++)
        {
            if (i < 0 || j < 0 || i > 10 || j > 100)
                continue;
            GameObject *gameObject = level[i][j];
            SDL_Rect *rect = gameObject->GetRectangle();

            // std::cout << "I: " << i << " J: " << j << std::endl;
            // std::cout << "X: " << i << " J: " << j << std::endl;
            // std::cout << "Tile ID: " << gameObject->GetTileId() << std::endl;

            switch (gameObject->GetTileId())
            {
            case StaticObject::EMPTY:
                if (SDL_HasIntersection(&(this->rect), rect))
                {
                    // std::cout << "Collision occured with empty space" << std::endl;
                }
                break;
            case StaticObject::WALL_RED:
                if (SDL_HasIntersection(&(this->rect), rect))
                {
                    std::cout << "Collision occured with wall at: " << i << ':' << j << std::endl;
                }
                break;
            default:
                break;
            }
        }
    }
}

Player::Player()
{
    x = 15;
    y = 5;
    this->SetRectDimension(16, 16);
    this->SetRectPosition(x, y);
    this->SetTileId(PlayerObject::PLAYER_FRONT);
}

void Player::MoveLeft()
{
    if (x - dx < 1)
        return;

    x -= dx;
    SetLeft();
    this->SetRectPosition(x, y);
}

void Player::MoveRight()
{
    if (x + dx > 99 * 16)
        return;

    x += dx;
    SetRight();
    this->SetRectPosition(x, y);
}

void Player::MoveUp()
{
    if (y - dy < 1)
        return;

    y -= dy;
    SetUp();
    this->SetRectPosition(x, y);
}

void Player::MoveDown()
{
    if (y + dy > 99 * 16)
        return;

    y += dy;
    SetDown();
    this->SetRectPosition(x, y);
}

SDL_bool Player::IsGrounded(TileManager *const &tileManager, std::array<std::array<GameObject *, 100>, 10> &level)
{
    int currX = this->rect.x;
    int currY = this->rect.y;

    int minCol = currX / 16;
    int maxCol = (currX + this->rect.w) / 16;
    int row = currY / 16;

    if (row >= 10)
        return SDL_TRUE;

    for (int col = minCol; col <= maxCol; ++col)
    {
        int tileId = level[row + 1][col]->GetTileId();

        switch (tileId)
        {
        case StaticObject::WALL_BLUE:
        case StaticObject::WALL_RED:
        case StaticObject::WALL_HALF_1:
        case StaticObject::WALL_HALF_2:
        case StaticObject::WALL_HALF_3:
        case StaticObject::WALL_HALF_4:
        case StaticObject::WALL_METAL:
        case StaticObject::WALL_MUD:
        case StaticObject::WALL_MUD_2:
        case StaticObject::WALL_LIGHT_BLUE:
        case StaticObject::DIVIDER_PURPLE:
        case StaticObject::PIPE_RIGHT:
        case StaticObject::PIPE_DOWN:
            return SDL_TRUE;
            break;
        default:

            break;
        }
    }
    return SDL_FALSE;
}

SDL_bool Player::IsColliding(std::array<std::array<GameObject *, 100>, 10> &level)
{
    GameState *gameState = GameState::getInstance();
    TileManager *tileManager = TileManager::getInstance();
    if (!tileManager)
    {
        SDL_Log("Tile manager not set\n");
        return SDL_FALSE;
    }
    int currX = this->rect.x;
    int currY = this->rect.y;
    int col = currX / 16;
    int row = currY / 16;
    int tileIndex = 0;
    SDL_Rect *other;
    int tileId = 0;
    int x = 0;
    int y = 0;

    if (this->currDir == DIR::LEFT)
    {
        x = row;
        y = col - 1;
    }
    else if (this->currDir == DIR::RIGHT)
    {
        x = row;
        y = col + 1;
    }
    else if (this->currDir == DIR::UP)
    {
        x = row - 1;
        y = col;
    }
    else if (this->currDir == DIR::DOWN)
    {
        x = row + 1;
        y = col;
    }

    if (x < 0 || y < 0)
        return SDL_FALSE;

    tileId = level[x][y]->GetTileId();
    other = level[x][y]->GetRectangle();

    switch (tileId)
    {
    case StaticObject::EMPTY:
        break;
    case StaticObject::WALL_BLUE:
    case StaticObject::WALL_RED:
    case StaticObject::WALL_HALF_1:
    case StaticObject::WALL_HALF_2:
    case StaticObject::WALL_HALF_3:
    case StaticObject::WALL_HALF_4:
    case StaticObject::WALL_METAL:
    case StaticObject::WALL_MUD:
    case StaticObject::WALL_MUD_2:
    case StaticObject::WALL_LIGHT_BLUE:
    case StaticObject::DIVIDER_PURPLE:
    case StaticObject::PIPE_RIGHT:
    case StaticObject::PIPE_DOWN:
        if (SDL_HasIntersection(&(this->rect), other))
        {
            std::cout << "Collision occured with a wall" << std::endl;
        }
        break;
    case StaticObject::POINT_BLUE:
        gameState->addScore(100);
        level[x][y]->SetTileId(StaticObject::EMPTY);
        break;
    case StaticObject::POINT_RED:
        gameState->addScore(200);
        level[x][y]->SetTileId(StaticObject::EMPTY);
        break;
    case StaticObject::POINT_PURPLE:
        gameState->addScore(300);
        level[x][y]->SetTileId(StaticObject::EMPTY);
        break;
    case StaticObject::POINT_RING:
        gameState->addScore(400);
        level[x][y]->SetTileId(StaticObject::EMPTY);
        break;
    case StaticObject::POINT_WAND:
        gameState->addScore(500);
        level[x][y]->SetTileId(StaticObject::EMPTY);
        break;
    case StaticObject::POINT_CROWN:
        gameState->addScore(800);
        level[x][y]->SetTileId(StaticObject::EMPTY);
        break;
    default:
        break;
    }

    return SDL_FALSE;
}

void Player::Gravity()
{
    if (JumpState())
    {
        jumpHeight += gravity;
        y += jumpHeight + gravity;
        if (jumpHeight > 0)
        {
            inJump = false;
            jumpHeight = -6;
        }
    }
    else
    {
        inJump = false;
        if (y + dy > 99 * 16)
            return;

        y += dy;
    }
    this->SetRectPosition(x, y);
}

void Player::GetJumpTime()
{
    jumpTimer = SDL_GetTicks();
}

bool Player::JumpState()
{
    return inJump;
}

void Player::Jump()
{
    if (jumpTimer - lastJump > 100)
    {
        inJump = true;
        lastJump = jumpTimer;
    }
    else
    {
        Gravity();
    }
}

void Player::SetLeft()
{
    currDir = DIR::LEFT;
}

void Player::SetRight()
{
    currDir = DIR::RIGHT;
}

void Player::SetUp()
{
    currDir = DIR::UP;
}

void Player::SetDown()
{
    currDir = DIR::DOWN;
}

int Player::GetDirection()
{
    return currDir;
}