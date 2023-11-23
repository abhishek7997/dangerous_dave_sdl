
#include "GameState.hpp"
#include "DigitDisplay.hpp"

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
    if (!this->jetpackActivated)
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
    if (this->player->IsGrounded() && this->player->canMoveUp())
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
    if (jetpackActivated)
    {
        this->player->ResetSpeed();
    }
    else
    {
        this->player->IncreaseSpeed();
    }
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
        instance = new GameState(renderer);
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
    this->digitDisplay = new DigitDisplay(renderer);
    this->player = new Player();
}

void GameState::RenderStates()
{
    // Display SCORES
    SDL_Texture *texture = TileManager::getInstance()->GetTileById(MiscObject::TEXT_SCORE);
    SDL_Rect dst = {0, 0, 62, 11};
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(renderer, texture, NULL, &dst);

    digitDisplay->RenderText(63, this->score);

    // Display Current level
    texture = TileManager::getInstance()->GetTileById(MiscObject::TEXT_LEVEL);
    int levelTextOffset = 62 + 5 * 8 + 8;
    dst = {levelTextOffset, 0, 62, 11};
    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(renderer, texture, NULL, &dst);

    digitDisplay->RenderText(levelTextOffset + 62, this->currentLevel + 1);

    // Display number of lives left
    texture = TileManager::getInstance()->GetTileById(MiscObject::TEXT_LIVES);
    int livesTextOffset = levelTextOffset + 62 + 8 * 3;

    dst = {livesTextOffset, 0, 62, 11};

    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(renderer, texture, NULL, &dst);

    for (int i = 0; i < this->lives; i++)
    {
        dst = {livesTextOffset + 62 + i * 17, 0, 17, 12};
        texture = TileManager::getInstance()->GetTileById(MiscObject::LIFE_UNIT);
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
        SDL_RenderCopy(renderer, texture, NULL, &dst);
    }

    // Display jetpack
    if (gotJetpack)
    {
        texture = TileManager::getInstance()->GetTileById(MiscObject::TEXT_JETPACK);
        dst = {0, 185, 62, 11};
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
        SDL_RenderCopy(renderer, texture, NULL, &dst);

        // Display jetpack meter
        texture = TileManager::getInstance()->GetTileById(MiscObject::JETPACK_METER);
        dst = {62 + 8, 185, 130, 12};
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
        SDL_RenderCopy(renderer, texture, NULL, &dst);
    }

    // Display gun if accquired
    if (gotGun)
    {
        texture = TileManager::getInstance()->GetTileById(MiscObject::TEXT_GUN);
        dst = {62 + 8 + 185, 185, 62, 11};
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
        SDL_RenderCopy(renderer, texture, NULL, &dst);

        texture = TileManager::getInstance()->GetTileById(StaticObject::GUN);
        dst = {62 + 8 + 185 + 62 + 8, 185, 62, 11};
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
        SDL_RenderCopy(renderer, texture, NULL, &dst);
    }

    // Display Go thru the door text
    if (gotTrophy)
    {
        texture = TileManager::getInstance()->GetTileById(MiscObject::TEXT_CANEXIT);
        dst = {62 + 8, 185 + 16, 176, 14};
        SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);
        SDL_RenderCopy(renderer, texture, NULL, &dst);
    }
}

void GameState::SetGotTrophy(bool status)
{
    this->gotTrophy = status;
}

void GameState::SetGotJetpack(bool status)
{
    this->gotJetpack = status;
}

void GameState::SetGotGun(bool status)
{
    this->gotGun = status;
}

bool GameState::GotGun()
{
    return this->gotGun;
}

bool GameState::GotTrophy()
{
    return this->gotTrophy;
}

bool GameState::GotJetpack()
{
    return this->gotJetpack;
}

void GameState::Reset()
{
    this->canClimb = false;
    this->gotJetpack = false;
    this->jetpackActivated = false;
    this->gotTrophy = false;
    this->canClimb = false;
    this->gotGun = false;
}

void GameState::DecreaseLives()
{
    this->lives--;
}

void GameState::NextLevel()
{
    this->currentLevel = std::min(9, this->currentLevel + 1);
}

void GameState::SetPlayerX(int x)
{
    this->GetPlayer()->SetPlayerX(x);
}

void GameState::SetPlayerY(int y)
{
    this->GetPlayer()->SetPlayerY(y);
}

void GameState::SetPlayerPos(int x, int y)
{
    this->GetPlayer()->SetPlayerPos(x, y);
}

GameState *GameState::instance = nullptr;