#pragma once

#include <SDL.h>
#include "GameObject.hpp"
#include "DigitDisplay.hpp"

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
    static GameState *getInstance();
    void AddScore(const int &score);
    void RenderStates();
    void SetGotTrophy(bool);
    void SetGotJetpack(bool);
    void SetGotGun(bool);
    bool GotTrophy();
    bool GotJetpack();
    bool GotGun();
    void Reset();
    void NextLevel();
    void SetPlayerX(int x);
    void SetPlayerY(int y);
    void SetPlayerPos(int x, int y);
    void DecreaseLives();
    void ConsumeJetpack();
    void Update();
    uint32_t GetTicks();

protected:
    static GameState *instance;
    GameState();

private:
    const int playerWidth = 20;
    const int playerHeight = 16;
    const float gravity = 0.4f;

    int lives = 3;
    int score = 0;
    int currentLevel = 2;

    bool gotJetpack = false;
    bool jetpackActivated = false;
    int jetpackFuel = 60;
    bool gotTrophy = false;
    bool canClimb = false;
    bool gotGun = false;

    uint32_t ticks;

    Player *player = nullptr;
    SDL_Renderer *renderer = nullptr;
    DigitDisplay *digitDisplay = nullptr;
};