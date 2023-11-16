#pragma once

#include <array>
#include <vector>
#include <utility>
#include <SDL.h>
#include "TileManager.hpp"
#include "tile_types.hpp"

class IGameObject
{
public:
    void Render(SDL_Renderer *renderer, const int &offset);
    void SetRectPosition(const int &x, const int &y);
    void SetRectDimension(const int &w, const int &h);
    int GetTileId();
    SDL_bool isColliding(const SDL_Rect *other);
    SDL_Rect *GetRectangle();
    void SetTileId(const int &tileId);

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
    void UpdateFrame(const int salt);
};

class MonsterObject : public IGameObject
{
public:
    MonsterObject() = delete;
    MonsterObject(const int &x, const int &y, const int &tileId);
    void Move();
    void SetMovements(const std::vector<std::pair<int, int>> &movements);
    SDL_bool IsColliding();

private:
    std::vector<std::pair<int, int>> movements;
    size_t size;
    std::vector<std::pair<int, int>>::iterator iterator;
    int startX = 0;
    int startY = 0;
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

    bool IsColliding(int x, int y);
    void IsColliding();
    void Gravity();
    void GetJumpTime();
    bool JumpState();
    void Jump();
    void SetLeft();
    void SetRight();
    void SetUp();
    void SetDown();
    int GetDirection();
    bool canMoveDown();
    bool canMoveUp();
    bool canMoveLeft();
    bool canMoveRight();
    bool canClimb();
    void PrintRectCoordinates();
    void IncreaseSpeed();
    void ResetSpeed();
    void UpdateFrame();
    void SetPlayerPos(int x, int y);
    void PlayDead();
    bool FiredBullet();
    void DestroyBullet();
    const SDL_Rect *GetBulletRect()
    {
        if (this->bullet != nullptr)
            return (this->bullet)->GetRectangle();
        return nullptr;
    }
    void FireBullet();
    void RenderBullet(SDL_Renderer *renderer, const int &offset);
    const Bullet *GetBullet();

private:
    int x, y;
    float gravity = 0.5f;
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
    double jumpHeight = -6;
    double jumpTimer;
    double lastJump = 0.0;
    bool collision_point[8] = {true};
    Bullet *bullet = nullptr;
};