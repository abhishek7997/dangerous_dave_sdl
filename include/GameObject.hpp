#pragma once

#include <array>
#include <vector>
#include <list>
#include <utility>
#include <SDL.h>
#include "TileManager.hpp"
#include "tile_types.hpp"

class IGameObject
{
public:
    void Render(const int &offset);
    void SetRectPosition(const int &x, const int &y);
    void SetRectDimension(const int &w, const int &h);
    int GetTileId();
    SDL_bool IsColliding(const SDL_Rect *other);
    SDL_Rect &GetRectangle();
    void SetTileId(const unsigned int &tileId);

protected:
    SDL_Rect rect;
    int tileId;
    int startTileId;
};

class GameObject : public IGameObject
{
public:
    GameObject() = delete;
    GameObject(const int &x, const int &y, const int &tileId);
    void SetPosition(const int &x, const int &y);
    void UpdateFrame(const int &salt);
    ~GameObject();
};

class EnemyBullet : public IGameObject
{
public:
    EnemyBullet();
    EnemyBullet(const int &dir, const int x, const int y);
    void UpdateFrame(const int &m_ticks);
    EnemyBullet &Get();
    static void RenderBullets();
    static void AddBullet(std::unique_ptr<EnemyBullet> b);
    void CheckPlayerCollision();

private:
    const int speed = 7;
    const int W = 20;
    const int H = 3;
    int dx;
    int dead_timer = 120;
    static std::list<std::unique_ptr<EnemyBullet>> bullets;
    static EnemyBullet instance;
};

class MonsterObject : public IGameObject
{
public:
    MonsterObject() = delete;
    MonsterObject(const int x, const int y, const int &tileId);
    void Move();
    void SetMovements(const std::vector<std::pair<int, int>> movements);
    SDL_bool IsColliding();
    void UpdateFrame();
    void FireBullet();
    bool InView();
    int GetDirection();
    ~MonsterObject();

private:
    int x;
    int y;
    int startX = 0;
    int startY = 0;
    size_t size;
    std::vector<std::pair<int, int>> movements;
    std::vector<std::pair<int, int>>::iterator iterator;
    uint32_t m_ticks = 0;
    const unsigned int fireRate = 20; // Higher value means low fire rate
};

class Bullet : public IGameObject
{
public:
    Bullet();
    Bullet(const int &dir, const int x, const int y);
    void UpdateFrame();

private:
    int dx = 4;
    int dead_timer = 120;
    const int W = 12;
    const int H = 3;
};

class Player : public IGameObject
{
public:
    Player();
    Player(const int &x, const int &y);
    void MoveLeft();
    void MoveRight();
    void MoveUp();
    void MoveDown();
    bool IsGrounded();
    bool IsColliding(const int &x, const int &y);
    void IsColliding();
    void Gravity();
    void GetJumpTime();
    bool JumpState();
    void Jump();
    void ResetJump();
    void SetLeft();
    void SetRight();
    void SetUp();
    void SetDown();
    int GetDirection();
    bool canMoveDown();
    bool canMoveUp();
    bool canMoveLeft();
    bool canMoveRight();
    void PrintRectCoordinates();
    void IncreaseSpeed();
    void ResetSpeed();
    void UpdateFrame();
    void SetPlayerX(const int &x);
    void SetPlayerY(const int &y);
    void SetPlayerPos(const int &x, const int &y);
    void PlayDead();
    bool FiredBullet();
    void DestroyBullet();
    bool IsDead();
    const SDL_Rect *GetBulletRect()
    {
        if (this->bullet != nullptr)
            return &(this->bullet)->GetRectangle();
        return nullptr;
    }
    void FireBullet();
    void RenderBullet(const int &offset);
    ~Player();

private:
    int x, y;
    double gravity = 0.5;
    int dx = 2;
    int dy = 2;
    int currDir = DIR::UNSET;
    bool can_move_left = true;
    bool can_move_right = true;
    bool can_move_up = true;
    bool can_move_down = true;
    bool isGrounded = false;
    bool climb = false;
    bool isDead = false;
    bool shoot = false;

    uint32_t player_tick = 0;
    uint32_t dead_timer = 70;
    bool inJump = false;
    double jumpHeight = -6.5;
    double jumpTimer = 0.0;
    double lastJump = 0.0;
    bool collision_point[8] = {true};
    std::unique_ptr<Bullet> bullet;
};