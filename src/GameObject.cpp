#include <array>
#include "GameState.hpp"
#include "LevelManager.hpp"

void IGameObject::Render(SDL_Renderer *renderer, const int &offset)
{
    TileManager *tileManager = TileManager::getInstance();
    if (!tileManager)
    {
        SDL_Log("TileManager not loaded");
        return;
    }
    SDL_Texture *texture = tileManager->GetTileById(this->tileId);
    if (!texture)
    {
        SDL_Log("Could not query texture :( %s", SDL_GetError());
        return;
    }
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

    SDL_Rect dst = {this->rect.x - offset, this->rect.y + 16, this->rect.w, this->rect.h};

    SDL_RenderCopy(renderer, texture, NULL, &dst);

    // if (this->tileId != 0)
    // {
    //     SDL_SetRenderDrawColor(renderer, 0xaa, 0x0, 0xff, 0xff);
    //     SDL_RenderDrawRect(renderer, &dst);
    // }
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
        this->startTileId = tileId;
        return;
    }
    this->tileId = tileId;
    this->startTileId = tileId;
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
    SetTileId(tileId);
}

void GameObject::UpdateFrame(const int salt)
{
    int mod = 1;
    switch (this->startTileId)
    {
    case 6:
        mod = 4;
        break;
    case 10:
        mod = 5;
        break;
    case 25:
        mod = 4;
        break;
    case 36:
        mod = 5;
        break;
    case 129:
        mod = 4;
        break;
    default:
        return;
    }

    this->tileId = this->startTileId + ((salt + (SDL_GetTicks() / 120)) % mod);
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

    this->startX = x;
    this->startY = y;
    this->SetRectPosition(x, y);
    this->SetRectDimension(tileWidth, tileHeight);
    this->SetTileId(tileId);
    this->movements = {{0, 12}, {1, 13}, {2, 14}, {3, 13}, {4, 12}, {5, 12}, {6, 13}, {7, 13}, {8, 13}, {9, 12}, {10, 12}, {10, 11}, {9, 11}, {8, 10}, {7, 9}, {6, 9}, {5, 10}, {4, 11}, {3, 11}, {2, 12}, {1, 12}, {0, 12}};
    this->size = this->movements.size();
    this->iterator = this->movements.begin();
}

void MonsterObject::Move()
{
    if (this->iterator == this->movements.end())
        this->iterator = this->movements.begin();
    int x = this->startX + (*(this->iterator)).first * 5;
    int y = this->startY + (*(this->iterator)).second * 5;
    SetRectPosition(x, y);
    this->iterator++;
}

void MonsterObject::SetMovements(const std::vector<std::pair<int, int>> &movements)
{
    this->movements = movements;
}

SDL_bool MonsterObject::IsColliding()
{
    TileManager *tileManager = TileManager::getInstance();
    LevelManager *levelManager = LevelManager::getInstance();

    if (!(tileManager && levelManager))
    {
        SDL_Log("Tile manager not set\n");
        return SDL_FALSE;
    }

    Level *level = levelManager->GetCurrentLevel();

    int currX = this->rect.x;
    int currY = this->rect.y;
    int cellX = currX / 16;
    int cellY = currY / 16;
    int tileIndex = 0;
    Player *player = GameState::getInstance()->GetPlayer();
    const SDL_Rect *playerRect = player->GetRectangle();

    if (SDL_HasIntersection(&this->rect, playerRect))
    {
        player->PlayDead();
        return SDL_TRUE;
    }

    if (player->FiredBullet())
    {
        const SDL_Rect *bulletRect = player->GetBulletRect();
        if (bulletRect == nullptr)
            return SDL_FALSE;
        if (SDL_HasIntersection(&this->rect, bulletRect))
        {
            player->DestroyBullet();
            return SDL_TRUE;
        }
    }
    return SDL_FALSE;
}

Bullet::Bullet()
{
    this->SetRectPosition(10, 5);
    this->SetRectDimension(this->W, this->H);
    this->SetTileId(PlayerObject::PLAYER_BULLET_R);
}
Bullet::Bullet(const int &dir, const int x, const int y)
{
    if (dir == DIR::LEFT)
    {
        this->SetTileId(PlayerObject::PLAYER_BULLET_L);
        this->dx = -4;
    }
    else if (dir == DIR::RIGHT)
    {
        this->SetTileId(PlayerObject::PLAYER_BULLET_R);
        this->dx = 4;
    }
    std::cout << "Bullet created at: " << x << ',' << y << std::endl;
    std::cout << "TileId: " << this->tileId << std::endl;
    this->SetRectPosition(x, y);
    this->SetRectDimension(this->W, this->H);
}

void Bullet::UpdateFrame()
{
    this->SetRectPosition(this->rect.x + this->dx, this->rect.y);
}

Player::Player()
{
    this->x = 15;
    this->y = 5;
    this->SetRectDimension(16, 16);
    this->SetRectPosition(x, y);
    this->SetTileId(PlayerObject::PLAYER_FRONT);
    this->bullet = nullptr;
}

Player::Player(const int &x, const int &y)
{
    this->x = x;
    this->y = y;
    this->SetRectDimension(16, 16);
    this->SetRectPosition(this->x, this->y);
    this->SetTileId(PlayerObject::PLAYER_FRONT);
    this->bullet = nullptr;
}

void Player::MoveLeft()
{
    if (!this->canMoveLeft() || this->x - this->dx < 1)
        return;

    this->x -= this->dx;
    this->SetLeft();
    this->SetRectPosition(this->x, this->y);
    this->player_tick++;
}

void Player::MoveRight()
{
    if (!this->canMoveRight() || this->x + this->dx > 99 * 16)
        return;

    this->x += this->dx;
    this->SetRight();
    this->SetRectPosition(this->x, this->y);
    this->player_tick++;
}

void Player::MoveUp()
{
    if (!this->canMoveUp() || this->y - this->dy < 1)
        return;

    this->y -= this->dy;
    this->SetUp();
    this->SetRectPosition(this->x, this->y);
    this->player_tick++;
}

void Player::MoveDown()
{
    if (!this->canMoveDown() || this->y + this->dy > 99 * 16)
        return;

    this->y += this->dy;
    this->SetDown();
    this->SetRectPosition(this->x, this->y);
    this->player_tick++;
}

// bool Player::IsGrounded(TileManager *const &tileManager, std::array<std::array<GameObject *, 100>, 10> &level)
bool Player::IsGrounded()
{
    return this->isGrounded;
}

// Player
bool Player::IsColliding(const int &px, const int &py)
{
    GameState *gameState = GameState::getInstance();
    TileManager *tileManager = TileManager::getInstance();
    LevelManager *levelManager = LevelManager::getInstance();

    if (!(tileManager && levelManager))
    {
        SDL_Log("Tile manager or level manager not set\n");
        return false;
    }

    Level *level = levelManager->getLevel(gameState->getCurrentLevel());

    int tileId = 0;
    int gridX = py / 16;
    int gridY = px / 16;
    bool res = false;

    if (gridX < 0 || gridX > 10 || gridY < 0 || gridY > 99)
        return true;

    SDL_Rect *rect = level->QueryCell(gridX, gridY);
    tileId = level->GetLevel()[gridX][gridY]->GetTileId();

    // if (tileId != 0)
    //     std::cout << "Colliding with tile: " << tileId << std::endl;
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
    case StaticObject::PLATFORM:
        res = true;
        break;
    case StaticObject::POINT_PURPLE:
        gameState->addScore(50);
        level->ClearCell(gridX, gridY);
        break;
    case StaticObject::POINT_BLUE:
        gameState->addScore(100);
        level->ClearCell(gridX, gridY);
        break;
    case StaticObject::POINT_RED:
        gameState->addScore(200);
        level->ClearCell(gridX, gridY);
        break;
    case StaticObject::POINT_RING:
        gameState->addScore(300);
        level->ClearCell(gridX, gridY);
        break;
    case StaticObject::POINT_WAND:
        gameState->addScore(500);
        level->ClearCell(gridX, gridY);
        break;
    case StaticObject::POINT_CROWN:
        gameState->addScore(800);
        level->ClearCell(gridX, gridY);
        break;
    case StaticObject::TROPHY_1:
    case StaticObject::TROPHY_2:
    case StaticObject::TROPHY_3:
    case StaticObject::TROPHY_4:
    case StaticObject::TROPHY_5:
        gameState->SetGotTrophy(true);
        gameState->addScore(1000);
        level->ClearCell(gridX, gridY);
        break;
    case StaticObject::JETPACK:
        gameState->SetGotJetpack(true);
        level->ClearCell(gridX, gridY);
        break;
    case StaticObject::GUN:
        gameState->SetGotGun(true);
        level->ClearCell(gridX, gridY);
        break;
    case StaticObject::DOOR:
        if (gameState->GotTrophy())
        {
            this->ResetSpeed();
            this->DestroyBullet();
            levelManager->NextLevel();
        }
        break;
    case StaticObject::FIRE_1:
    case StaticObject::FIRE_2:
    case StaticObject::FIRE_3:
    case StaticObject::FIRE_4:
    case StaticObject::WATER_1:
    case StaticObject::WATER_2:
    case StaticObject::WATER_3:
    case StaticObject::WATER_4:
    case StaticObject::WEED_1:
    case StaticObject::WEED_2:
    case StaticObject::WEED_3:
    case StaticObject::WEED_4:
        res = false;
        this->PlayDead();
        break;
    default:
        break;
    }

    return res;
}

void Player::IsColliding()
{

    // std::cout << this->x / 16 << ' ' << this->y / 16 << std::endl;
    this->collision_point[0] = this->IsColliding(this->rect.x + 4, this->rect.y - 1);   // Top left
    this->collision_point[1] = this->IsColliding(this->rect.x + 10, this->rect.y - 1);  // Top right
    this->collision_point[2] = this->IsColliding(this->rect.x + 13, this->rect.y + 3);  // Right side above mid
    this->collision_point[3] = this->IsColliding(this->rect.x + 13, this->rect.y + 13); // Right side below mid
    this->collision_point[4] = this->IsColliding(this->rect.x + 10, this->rect.y + 17); // Right foot
    this->collision_point[5] = this->IsColliding(this->rect.x + 4, this->rect.y + 17);  // Left foot
    this->collision_point[6] = this->IsColliding(this->rect.x + 4, this->rect.y + 13);  // Left side below mid
    this->collision_point[7] = this->IsColliding(this->rect.x + 2, this->rect.y + 3);   // Left side above mid

    // for (int i = 0; i < 8; i++)
    // {
    //     std::cout << this->collision_point[i] << ' ';
    // }
    // std::cout << std::endl;
    this->isGrounded = this->collision_point[4] || this->collision_point[5];

    if (this->isGrounded)
    {
        int align = this->y % 16;
        if (align)
            this->y = (align < 8) ? this->y - align : this->y + 16 - align;
    }
}

void Player::Gravity()
{
    if (this->isDead)
        return;
    if (this->inJump)
    {
        this->jumpHeight += gravity;
        this->y += jumpHeight + gravity;
        if (this->jumpHeight > 0.0 || !canMoveUp())
        {
            this->inJump = false;
            this->jumpHeight = -6.5;
        }
    }
    else
    {
        if (this->y + this->dy < 99 * 16)
            this->y += this->dy;
    }
    this->SetRectPosition(this->x, this->y);
}

void Player::GetJumpTime()
{
    this->jumpTimer = SDL_GetTicks();
}

bool Player::JumpState()
{
    return this->inJump;
}

void Player::Jump()
{
    if (this->jumpTimer - this->lastJump > 100)
    {
        this->inJump = true;
        this->lastJump = this->jumpTimer;
    }
    else
    {
        this->Gravity();
    }
}

void Player::SetLeft()
{
    this->currDir = DIR::LEFT;
}

void Player::SetRight()
{
    this->currDir = DIR::RIGHT;
}

void Player::SetUp()
{
    this->currDir = DIR::UP;
}

void Player::SetDown()
{
    this->currDir = DIR::DOWN;
}

bool Player::canMoveDown()
{
    return !(this->collision_point[4] && this->collision_point[5]) && !this->isDead;
}

bool Player::canMoveUp()
{
    return !(this->collision_point[0] || this->collision_point[1]) && !this->isDead;
}

bool Player::canMoveLeft()
{
    return !(this->collision_point[6] || this->collision_point[7]) && !this->isDead;
}

bool Player::canMoveRight()
{
    return !(this->collision_point[2] || this->collision_point[3]) && !this->isDead;
}

int Player::GetDirection()
{
    return this->currDir;
}

void Player::PrintRectCoordinates()
{
    SDL_Log("Player at X: %d , Y: %d\n", this->rect.x / 16, this->rect.y / 16);
}

void Player::IncreaseSpeed()
{
    this->dx = 4;
    this->dy = 4;
}

void Player::ResetSpeed()
{
    this->dx = 2;
    this->dy = 2;
}

void Player::UpdateFrame()
{
    if (this->bullet != nullptr)
    {
        this->bullet->UpdateFrame();
        if (this->GetBulletRect()->x - LevelManager::getInstance()->GetCurrentOffset() < 0 || this->GetBulletRect()->x - LevelManager::getInstance()->GetCurrentOffset() > 20 * 16)
            this->DestroyBullet();
    }
    if (this->isDead)
    {
        if (this->dead_timer == 0)
        {
            this->isDead = false;
            this->tileId = this->startTileId;
            this->dead_timer = 70;
            LevelManager::getInstance()->ResetPlayerPos();
            LevelManager::getInstance()->ResetOffset();
            GameState::getInstance()->DecreaseLives();
        }
        else
        {
            this->tileId = MiscObject::DEATH_1 + ((this->dead_timer / 5) % 4);
            if (this->dead_timer < 25)
            {
                this->tileId = StaticObject::EMPTY;
            }
            this->dead_timer--;
        }
    }
    else if (this->inJump || !this->IsGrounded())
    {
        if (this->GetDirection() == DIR::UNSET)
            this->tileId = PlayerObject::PLAYER_FRONT;
        else if (GameState::getInstance()->jetpackState())
        {
            this->tileId = (this->GetDirection() == DIR::RIGHT) ? (PlayerObject::PLAYER_JETPACK_R_1 + ((this->player_tick / 3) % 3)) : (PlayerObject::PLAYER_JETPACK_L_1 + ((this->player_tick / 3) % 3));
        }
        else
        {
            this->tileId = this->GetDirection() == DIR::RIGHT ? PlayerObject::PLAYER_JUMP_R : PlayerObject::PLAYER_JUMP_L;
        }
    }
    else
    {
        this->tileId = this->startTileId + (this->player_tick / 3) % 3;
    }
}

void Player::SetPlayerX(const int &x)
{
    this->x = x;
    this->SetRectPosition(this->x, this->y);
}

void Player::SetPlayerY(const int &y)
{
    this->y = y;
    this->SetRectPosition(this->x, this->y);
}

void Player::SetPlayerPos(const int &x, const int &y)
{
    this->x = x;
    this->y = y;
    this->SetRectPosition(this->x, this->y);
    std::cout << "Set player pos to: " << this->x << ',' << this->y << std::endl;
}

void Player::PlayDead()
{
    this->isDead = true;
}

bool Player::FiredBullet()
{
    return (this->bullet != nullptr);
}

void Player::FireBullet()
{
    if (!GameState::getInstance()->GotGun() || this->bullet != nullptr)
        return;
    this->bullet = new Bullet(this->GetDirection(), this->x, this->y);
}

void Player::RenderBullet(SDL_Renderer *renderer, const int &offset)
{
    if (this->bullet == nullptr)
    {
        return;
    }
    this->bullet->Render(renderer, offset);
}

void Player::DestroyBullet()
{
    delete bullet;
    // bullet = nullptr;
}

const Bullet *Player::GetBullet()
{
    return this->bullet;
}