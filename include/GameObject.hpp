#pragma once

#include <vector>
#include <utility>
#include <SDL.h>
#include "TileManager.hpp"
#include "../src/tile_types.cpp"

class IGameObject
{
public:
    virtual void Render(SDL_Renderer *renderer, TileManager *tileManager) const
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
    virtual void SetRectPosition(const int &x, const int &y)
    {
        (this->rect).x = x;
        (this->rect).y = y;
    };

    virtual void SetRectDimension(const int &w, const int &h)
    {
        (this->rect).w = w;
        (this->rect).h = h;
    }

    virtual int GetTileId()
    {
        return this->tileId;
    }

    virtual SDL_bool isColliding(const SDL_Rect *other)
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

    virtual void SetTileId(const int &tileId)
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

    SDL_bool IsColliding(TileManager *const &tileManager, GameObject *(*level)[10][20])
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
                GameObject *gameObject = (*level)[i][j];
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

class Player : IGameObject
{
public:
    Player()
    {
        SetTileId(PlayerObject::PLAYER_FRONT);
    }

private:
    using IGameObject::GetTileId;
    using IGameObject::SetTileId;
};