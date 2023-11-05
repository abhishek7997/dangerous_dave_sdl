#pragma once

#include <SDL.h>
#include "GameObject.hpp"

class GameState
{
public:
    void moveLeft();
    void moveRight();
    void moveDown();
    void moveUp();
    void applyGravity();
    void jump();
    void playerAnimation();
    void SetPlayer(Player *player);
    Player *&GetPlayer();
    void toggleJetpack();
    bool jetpackState();
    int getCurrentLevel();
    void SetRenderer(SDL_Renderer *renderer);
    static GameState *getInstance();
    static GameState *getInstance(SDL_Renderer *renderer);
    void addScore(int score);

protected:
    static GameState *instance;
    GameState();
    GameState(SDL_Renderer *renderer);

private:
    const int playerWidth = 20;
    const int playerHeight = 16;
    const float gravity = 0.5f;

    int lives = 3;
    int score = 0;
    int currentLevel = 0;
    int tileOffset = 0;

    bool jetpackActivated = false;
    bool gotTrophy = false;
    bool canClimb = false;

    Player *player = nullptr;
    SDL_Renderer *renderer = nullptr;
};