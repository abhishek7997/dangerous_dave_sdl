#pragma once

#include <array>
#include <vector>
#include <utility>
#include <SDL.h>
#include "TileManager.hpp"
#include "../src/tile_types.cpp"

class IGameObject
{
public:
    void Render(SDL_Renderer *renderer, const int &offset) const;
    void SetRectPosition(const int &x, const int &y);
    void SetRectDimension(const int &w, const int &h);
    int GetTileId();
    SDL_bool isColliding(const SDL_Rect *other);
    SDL_Rect *GetRectangle();
    void SetTileId(const int &tileId);

protected:
    SDL_Rect rect;
    int tileId;
};

class GameObject : public IGameObject
{
public:
    GameObject() = delete;
    GameObject(const int &x, const int &y, const int &tileId);
    void SetPosition(const int &x, const int &y);
};

class MonsterObject : public IGameObject
{
public:
    MonsterObject() = delete;
    MonsterObject(const int &x, const int &y, const int &tileId);
    void Move();
    void SetMovements(std::vector<std::pair<int, int>> movements);
    SDL_bool IsColliding(std::array<std::array<GameObject *, 100>, 10> &level);

private:
    std::vector<std::pair<int, int>> movements;
    size_t size;
    std::vector<std::pair<int, int>>::iterator iterator;
};

class Player : public IGameObject
{
public:
    Player();
    void MoveLeft();
    void MoveRight();
    void MoveUp();
    void MoveDown();
    SDL_bool IsGrounded(TileManager *const &tileManager, std::array<std::array<GameObject *, 100>, 10> &level);

    SDL_bool IsColliding(std::array<std::array<GameObject *, 100>, 10> &level);
    void Gravity();
    void GetJumpTime();
    bool JumpState();
    void Jump();
    void SetLeft();
    void SetRight();
    void SetUp();
    void SetDown();
    int GetDirection();

private:
    int x, y;
    float gravity = 0.5f;
    int dx = 2;
    int dy = 2;
    int currDir = 0;

    bool inJump = false;
    double jumpHeight = -6;
    double jumpTimer;
    double lastJump = 0;
};