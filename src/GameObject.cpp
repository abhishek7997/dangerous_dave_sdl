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

    // Draw the bounding boxes of the texture
    // if (this->tileId != 0)
    // {
    //     SDL_SetRenderDrawColor(renderer, 0xaa, 0x0, 0xff, 0xff);
    //     SDL_RenderDrawRect(renderer, &dst);
    // }
};

void IGameObject::SetRectPosition(const int &x, const int &y)
{
    this->rect.x = x;
    this->rect.y = y;
};

void IGameObject::SetRectDimension(const int &w, const int &h)
{
    this->rect.w = w;
    this->rect.h = h;
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

void GameObject::UpdateFrame(const int &salt)
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
MonsterObject::MonsterObject(const int x, const int y, const int tileId)
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
    this->x = x;
    this->y = y;
    this->SetRectPosition(x, y);
    this->SetRectDimension(tileWidth, tileHeight);
    this->SetTileId(tileId);
    this->movements = {{1, 1}, {1, 1}, {-1, -1}, {-1, -1}};
    this->size = this->movements.size();
    this->iterator = this->movements.begin();
}

MonsterObject::~MonsterObject()
{
    for (auto it = this->bullets.begin(); it != this->bullets.end(); ++it)
    {
        delete *it;
    }
    this->bullets.clear();
}

bool MonsterObject::InView()
{
    const int playerOff = GameState::getInstance()->GetPlayer()->GetRectangle()->x / (20 * 16);
    const int monsOff = this->x / (20 * 16);

    return ((this->GetRectangle()->x >= 0) && (this->GetRectangle()->x <= SCREENOFFSET::FOUR) && playerOff == monsOff);
}

void MonsterObject::Move()
{
    if (this->iterator == this->movements.end())
    {
        this->x = this->startX;
        this->y = this->startY;
        this->iterator = this->movements.begin();
    }
    this->x += this->iterator->first * 8;
    this->y += this->iterator->second * 8;
    this->SetRectPosition(this->x, this->y);
    this->iterator++;
}

void MonsterObject::UpdateFrame()
{
    this->tileId = this->startTileId + (this->m_ticks / 2) % 4;
    this->m_ticks++;
    if (this->m_ticks % 2 == 0)
    {
        this->Move();
    }
    else
    {
        this->FireBullet();
        for (auto it = this->bullets.begin(); it != this->bullets.end();)
        {
            (*it)->UpdateFrame(this->m_ticks);
            if ((*it)->GetRectangle()->x < 2 || (*it)->GetRectangle()->x > 16 * 99)
            {
                it = this->DestroyBullet(it);
            }
            else
            {
                ++it;
            }
        }
    }
}

int MonsterObject::GetDirection()
{
    const int playerX = GameState::getInstance()->GetPlayer()->GetRectangle()->x;
    if (playerX < this->x)
        return DIR::LEFT;
    return DIR::RIGHT;
}

void MonsterObject::FireBullet()
{
    if (this->InView() && ((this->m_ticks / 3) % this->fireRate == 0) && this->bullets.size() < 6)
    {
        int x;
        switch (this->GetDirection())
        {
        case DIR::LEFT:
            x = this->x - 2;
            break;
        case DIR::RIGHT:
            x = this->x + 12;
            break;
        default:
            x = this->x + 12;
            break;
        }
        this->bullets.push_back(new EnemyBullet(this->GetDirection(), x, this->y + 8));
    }
}

void MonsterObject::RenderBullet(SDL_Renderer *renderer, const int &offset)
{
    for (auto bullet : this->bullets)
    {
        bullet->Render(renderer, offset);
    }
}

std::list<EnemyBullet *>::iterator MonsterObject::DestroyBullet(std::list<EnemyBullet *>::iterator it)
{
    delete *it;
    return bullets.erase(it);
}

void MonsterObject::SetMovements(const std::vector<std::pair<int, int>> movements)
{
    this->movements = movements;
    this->iterator = this->movements.begin();
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

    if (SDL_HasIntersection(&this->rect, playerRect) && !player->IsDead())
    {
        player->PlayDead();
        return SDL_TRUE;
    }

    for (auto it = this->bullets.begin(); it != this->bullets.end();)
    {
        if (*it != nullptr && SDL_HasIntersection((*it)->GetRectangle(), playerRect))
        {
            player->PlayDead();
            it = this->DestroyBullet(it);
            return SDL_FALSE;
        }
        else
        {
            ++it;
        }
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
    this->SetRectPosition(x, y);
    this->SetRectDimension(this->W, this->H);
}

void Bullet::UpdateFrame()
{
    this->SetRectPosition(this->rect.x + this->dx, this->rect.y);
}

EnemyBullet::EnemyBullet()
{
    this->SetRectPosition(10, 5);
    this->SetRectDimension(this->W, this->H);
    this->SetTileId(EnemyObject::ENEMY_BULLET_R_1);
}

EnemyBullet::EnemyBullet(const int &dir, const int x, const int y)
{
    if (dir == DIR::LEFT)
    {
        this->SetTileId(EnemyObject::ENEMY_BULLET_L_1);
        this->dx = -speed;
    }
    else if (dir == DIR::RIGHT)
    {
        this->SetTileId(EnemyObject::ENEMY_BULLET_R_1);
        this->dx = speed;
    }
    this->SetRectPosition(x, y);
    this->SetRectDimension(this->W, this->H);
}

void EnemyBullet::UpdateFrame(const int &m_ticks)
{
    this->tileId = this->startTileId + (m_ticks / 4) % 3;
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
        gameState->AddScore(50);
        level->ClearCell(gridX, gridY);
        break;
    case StaticObject::POINT_BLUE:
        gameState->AddScore(100);
        level->ClearCell(gridX, gridY);
        break;
    case StaticObject::POINT_RED:
        gameState->AddScore(200);
        level->ClearCell(gridX, gridY);
        break;
    case StaticObject::POINT_RING:
        gameState->AddScore(300);
        level->ClearCell(gridX, gridY);
        break;
    case StaticObject::POINT_WAND:
        gameState->AddScore(500);
        level->ClearCell(gridX, gridY);
        break;
    case StaticObject::POINT_CROWN:
        gameState->AddScore(800);
        level->ClearCell(gridX, gridY);
        break;
    case StaticObject::TROPHY_1:
    case StaticObject::TROPHY_2:
    case StaticObject::TROPHY_3:
    case StaticObject::TROPHY_4:
    case StaticObject::TROPHY_5:
        gameState->SetGotTrophy(true);
        gameState->AddScore(1000);
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
    this->collision_point[0] = this->IsColliding(this->rect.x + 3, this->rect.y - 1);   // Top left
    this->collision_point[1] = this->IsColliding(this->rect.x + 11, this->rect.y - 1);  // Top right
    this->collision_point[2] = this->IsColliding(this->rect.x + 14, this->rect.y + 3);  // Right side above mid
    this->collision_point[3] = this->IsColliding(this->rect.x + 14, this->rect.y + 13); // Right side below mid
    this->collision_point[4] = this->IsColliding(this->rect.x + 11, this->rect.y + 17); // Right foot
    this->collision_point[5] = this->IsColliding(this->rect.x + 3, this->rect.y + 17);  // Left foot
    this->collision_point[6] = this->IsColliding(this->rect.x - 1, this->rect.y + 13);  // Left side below mid
    this->collision_point[7] = this->IsColliding(this->rect.x - 1, this->rect.y + 3);   // Left side above mid

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
            this->ResetJump();
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

void Player::ResetJump()
{
    this->inJump = false;
    this->jumpHeight = -6.5;
    this->lastJump = 0.0;
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
        const int currOffset = LevelManager::getInstance()->GetCurrentOffset() / SCREENOFFSET::ONE;
        const int bullOffset = this->GetBulletRect()->x / SCREENOFFSET::ONE;

        if ((this->GetBulletRect()->x < 0) || (this->GetBulletRect()->x > 17 * 99) || bullOffset != currOffset)
        {
            this->DestroyBullet();
        }
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

    if (GameState::getInstance()->jetpackState())
    {
        if ((GameState::getInstance()->GetTicks()) % 6 == 0)
        {
            GameState::getInstance()->ConsumeJetpack();
        }
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
}

void Player::PlayDead()
{
    this->isDead = true;
}

bool Player::IsDead()
{
    return this->isDead;
}

bool Player::FiredBullet()
{
    return (this->bullet != nullptr);
}

void Player::FireBullet()
{
    if (!GameState::getInstance()->GotGun() || this->bullet != nullptr || this->isDead)
        return;
    int x;
    switch (this->GetDirection())
    {
    case DIR::UNSET:
    case DIR::LEFT:
        x = this->x - 2;
        break;
    case DIR::RIGHT:
        x = this->x + 12;
        break;
    default:
        x = this->x + 12;
        break;
    }
    this->bullet = new Bullet(this->GetDirection(), x, this->y + 8);
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
    bullet = nullptr;
}

const Bullet *Player::GetBullet()
{
    return this->bullet;
}