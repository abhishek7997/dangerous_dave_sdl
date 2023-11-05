
#include "GameState.hpp"

void GameState::moveLeft()
{
    this->player->MoveLeft();
}

void GameState::moveRight()
{
    this->player->MoveRight();
}

void GameState::moveDown()
{
    if (!jetpackActivated)
        return;
    this->player->MoveDown();
}

void GameState::moveUp()
{
    this->player->MoveUp();
}

void GameState::applyGravity()
{
    if (jetpackActivated)
        return;
    this->player->Gravity();
}

void GameState::jump()
{
    this->player->Jump();
}

void GameState::playerAnimation()
{
}

void GameState::SetPlayer(Player *player)
{
    this->player = player;
}

Player *&GameState::GetPlayer()
{
    return this->player;
}

void GameState::toggleJetpack()
{
    jetpackActivated = !jetpackActivated;
}

bool GameState::jetpackState()
{
    return jetpackActivated;
}

int GameState::getCurrentLevel()
{
    return currentLevel;
}

void GameState::SetRenderer(SDL_Renderer *renderer)
{
    this->renderer = renderer;
}

GameState *GameState::getInstance()
{
    if (instance == nullptr)
    {
        instance = new GameState();
    }
    return instance;
}

GameState *GameState::getInstance(SDL_Renderer *renderer)
{
    if (instance == nullptr)
    {
        instance = new GameState();
    }
    return instance;
}

void GameState::addScore(int score)
{
    this->score += score;
}

GameState::GameState()
{
    this->player = new Player();
}

GameState::GameState(SDL_Renderer *renderer)
{
    this->renderer = renderer;
    this->player = new Player();
}

GameState *GameState::instance = nullptr;