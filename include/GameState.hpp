#pragma once

#include <SDL.h>
#include "GameObject.hpp"

class GameState
{
public:
    GameState()
    {
        this->player = new Player();
    }

    void moveLeft()
    {
        this->player->MoveLeft();
    }

    void moveRight()
    {
        this->player->MoveRight();
    }

    void moveDown()
    {
        if (jetpackActivated)
            return;
        this->player->MoveDown();
    }

    void moveUp()
    {
        this->player->MoveUp();
    }

    void applyGravity()
    {
        if (jetpackActivated)
            return;
        this->player->Gravity();
    }

    void jump()
    {
        this->player->Jump();
    }

    void playerAnimation()
    {
    }

    void SetPlayer(Player *player)
    {
        this->player = player;
    }

    Player *&GetPlayer()
    {
        return this->player;
    }

    void toggleJetpack()
    {
        jetpackActivated = !jetpackActivated;
    }

    bool jetpackState()
    {
        return jetpackActivated;
    }

private:
    const int playerWidth = 20;
    const int playerHeight = 16;
    const float gravity = 0.5f;

    int posx = 2;
    int posy = 2;
    int lives = 3;
    int score = 0;
    int currentLevel;
    int tileOffset = 0;

    bool jetpackActivated = false;
    bool gotTrophy = false;
    bool canClimb = false;

    Player *player = nullptr;
};