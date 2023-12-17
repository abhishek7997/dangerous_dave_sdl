#pragma once

#include <SDL.h>
#include "GameObject.hpp"
#include "DigitDisplay.hpp"

class GameState
{
public:
    static GameState &Get();
    void ApplyGravity();
    std::shared_ptr<Player> GetPlayer();
    void ToggleJetpack();
    bool IsJetpackActivated();
    int getCurrentLevel();
    void AddScore(const int &score);
    void RenderStates();
    void SetGotTrophy(const bool &status);
    void SetGotJetpack(const bool &status);
    void SetGotGun(const bool &status);
    bool GotTrophy();
    bool GotJetpack();
    bool GotGun();
    void Reset();
    void NextLevel();
    void SetPlayerX(const int &x);
    void SetPlayerY(const int &y);
    void SetPlayerPos(const int &x, const int &y);
    void DecreaseLives();
    void ConsumeJetpack();
    void Update();
    int GetCurrentLevelOffset();
    unsigned int GetTicks();
    ~GameState();
    GameState(const GameState &) = delete;
    // GameState &operator=(const GameState &) = delete;

private:
    GameState();

    const int playerWidth = 20;
    const int playerHeight = 16;
    const float gravity = 0.4f;

    unsigned short lives = 3;
    unsigned int score = 0;
    unsigned short currentLevel = 2;
    unsigned jetpackFuel = 60;

    bool gotJetpack = false;
    bool jetpackActivated = false;
    bool gotTrophy = false;
    bool canClimb = false;
    bool gotGun = false;

    unsigned int ticks;

    std::shared_ptr<Player> player;
    std::shared_ptr<SDL_Renderer> renderer;
    std::unique_ptr<DigitDisplay> digitDisplay;
};