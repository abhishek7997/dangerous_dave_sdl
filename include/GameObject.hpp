#pragma once

#include <vector>
#include <utility>
#include <SDL.h>
#include "TileManager.hpp"
#include "../src/tile_types.cpp"

class IGameObject
{
public:
    void Render(SDL_Renderer *renderer, TileManager *tileManager) const
    {
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
        SDL_RenderCopy(renderer, texture, NULL, &(this->rect));
        if (tileId != 0)
        {
            SDL_SetRenderDrawColor(renderer, 0xff, 0x0, 0xff, 0xff);
            SDL_RenderDrawRect(renderer, &(this->rect));
        }
    };

    void SetRectPosition(const int &x, const int &y)
    {
        (this->rect).x = x;
        (this->rect).y = y;
    };

    void SetRectDimension(const int &w, const int &h)
    {
        (this->rect).w = w;
        (this->rect).h = h;
    }

    int GetTileId()
    {
        return this->tileId;
    }

    SDL_bool isColliding(const SDL_Rect *other)
    {
        return SDL_HasIntersection(&(this->rect), other);
    }

    SDL_Rect *GetRectangle()
    {
        return &(this->rect);
    }

protected:
    SDL_Rect rect;
    int tileId;

    void SetTileId(const int &tileId)
    {
        if (tileId < 0 || tileId > 157)
        {
            this->tileId = 0;
            return;
        }
        this->tileId = tileId;
    }
};

class GameObject : public IGameObject
{
public:
    GameObject() = delete;
    GameObject(const int &x, const int &y, const int &tileId, TileManager *tileManager)
    {
        int tileWidth, tileHeight;
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

    void SetPosition(const int &x, const int &y)
    {
        this->rect.x = x;
        this->rect.y = y;
    }
};

class MonsterObject : public IGameObject
{
public:
    MonsterObject() = delete;
    MonsterObject(const int &x, const int &y, const int &tileId, TileManager *tileManager)
    {
        int tileWidth, tileHeight;
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

    void Move()
    {
        if (this->iterator == this->movements.end())
            this->iterator = this->movements.begin();
        int x = (*(this->iterator)).first * 3;
        int y = (*(this->iterator)).second * 3;
        SetRectPosition(x, y);
        this->iterator++;
    }

    void SetMovements(std::vector<std::pair<int, int>> movements)
    {
        this->movements = movements;
    }

    SDL_bool IsColliding(TileManager *const &tileManager, std::array<std::array<GameObject *, 20>, 10> &level)
    {
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
                if (i < 0 || j < 0 || i > 10 || j > 20)
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

private:
    std::vector<std::pair<int, int>> movements;
    size_t size;
    std::vector<std::pair<int, int>>::iterator iterator;
};

class Player : public IGameObject
{
public:
    Player()
    {
        x = 15;
        y = 5;
        this->SetRectDimension(16, 16);
        this->SetRectPosition(x, y);
        this->SetTileId(PlayerObject::PLAYER_FRONT);
    }

    void MoveLeft()
    {
        if (x - dx < 1)
            return;

        x -= dx;
        this->SetRectPosition(x, y);
    }

    void MoveRight()
    {
        if (x + dx > 19 * 16)
            return;

        x += dx;
        this->SetRectPosition(x, y);
    }

    void MoveUp()
    {
        if (y - dy < 1)
            return;

        y -= dy;
        this->SetRectPosition(x, y);
    }

    void MoveDown()
    {
        if (y + dy > 19 * 16)
            return;

        y += dy;
        this->SetRectPosition(x, y);
    }

    SDL_bool IsGrounded(TileManager *const &tileManager, std::array<std::array<GameObject *, 20>, 10> &level)
    {
        int currX = this->rect.x;
        int currY = this->rect.y;

        int col = currX / 16;
        int row = currY / 16;

        if (row >= 10)
            return SDL_TRUE;

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
            return SDL_FALSE;
            break;
        }
    }

    SDL_bool IsColliding(TileManager *const &tileManager, std::array<std::array<GameObject *, 20>, 10> &level)
    {
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
                if (i < 0 || j < 0 || i > 10 || j > 20)
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
                case StaticObject::WALL_BLUE:
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

    void Gravity()
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
            if (y + dy > 19 * 16)
                return;

            y += dy;
        }
        this->SetRectPosition(x, y);
    }

    void GetJumpTime()
    {
        jumpTimer = SDL_GetTicks();
    }

    bool JumpState()
    {
        return inJump;
    }

    void Jump()
    {
        if (jumpTimer - lastJump > 100)
        {
            std::cout << "Set jump" << std::endl;
            inJump = true;
            lastJump = jumpTimer;
        }
        else
        {
            std::cout << "No jump" << std::endl;
            Gravity();
        }
    }

private:
    using IGameObject::GetTileId;
    using IGameObject::SetTileId;

    int x, y;
    float gravity = 0.5f;
    int dx = 2;
    int dy = 2;

    bool inJump = false;
    double jumpHeight = -6;
    double jumpTimer;
    double lastJump = 0;
};