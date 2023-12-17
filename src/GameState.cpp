
#include "GameState.hpp"

GameState::GameState() : ticks(0), player(new Player()), renderer(SDLApp::Get().GetRenderer()), digitDisplay(new DigitDisplay())
{
}

GameState &GameState::Get()
{
    static GameState instance;
    return instance;
}

void GameState::AddScore(const int &score)
{
    this->score += score;
}

void GameState::ApplyGravity()
{
    if (jetpackActivated)
        return;
    this->player->Gravity();
}

std::shared_ptr<Player> GameState::GetPlayer()
{
    return this->player;
}

void GameState::ToggleJetpack()
{
    if (this->jetpackFuel == 0)
    {
        return;
    }
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

void GameState::ConsumeJetpack()
{
    if (this->jetpackFuel > 0 && !this->player->IsDead())
    {
        --(this->jetpackFuel);
    }
    else
    {
        this->player->ResetSpeed();
        this->jetpackActivated = false;
    }
}

bool GameState::IsJetpackActivated()
{
    return jetpackActivated;
}

int GameState::getCurrentLevel()
{
    return currentLevel;
}

void GameState::RenderStates()
{
    // Display SCORES
    std::shared_ptr<SDL_Texture> texture = TileManager::Get().GetTileById(MiscObject::TEXT_SCORE);
    SDL_Rect dst = {0, 0, 62, 11};
    SDL_SetTextureBlendMode(texture.get(), SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(renderer.get(), texture.get(), NULL, &dst);

    digitDisplay->RenderText(63, this->score);

    // Display Current level
    texture = TileManager::Get().GetTileById(MiscObject::TEXT_LEVEL);
    int levelTextOffset = 62 + 5 * 8 + 8;
    dst = {levelTextOffset, 0, 62, 11};
    SDL_SetTextureBlendMode(texture.get(), SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(renderer.get(), texture.get(), NULL, &dst);

    digitDisplay->RenderText(levelTextOffset + 62, this->currentLevel + 1);

    // Display number of lives left
    texture = TileManager::Get().GetTileById(MiscObject::TEXT_LIVES);
    int livesTextOffset = levelTextOffset + 62 + 8 * 3;

    dst = {livesTextOffset, 0, 62, 11};

    SDL_SetTextureBlendMode(texture.get(), SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(renderer.get(), texture.get(), NULL, &dst);

    for (int i = 0; i < this->lives; i++)
    {
        dst = {livesTextOffset + 62 + i * 17, 0, 17, 12};
        texture = TileManager::Get().GetTileById(MiscObject::LIFE_UNIT);
        SDL_SetTextureBlendMode(texture.get(), SDL_BLENDMODE_BLEND);
        SDL_RenderCopy(renderer.get(), texture.get(), NULL, &dst);
    }

    // Display jetpack
    if (gotJetpack)
    {
        texture = TileManager::Get().GetTileById(MiscObject::TEXT_JETPACK);
        dst = {0, 185, 62, 11};
        SDL_SetTextureBlendMode(texture.get(), SDL_BLENDMODE_BLEND);
        SDL_RenderCopy(renderer.get(), texture.get(), NULL, &dst);

        // Display jetpack meter
        texture = TileManager::Get().GetTileById(MiscObject::JETPACK_METER);
        dst = {62 + 8, 185, 130, 12};
        SDL_SetTextureBlendMode(texture.get(), SDL_BLENDMODE_BLEND);
        SDL_RenderCopy(renderer.get(), texture.get(), NULL, &dst);

        // Display jetpack units
        texture = TileManager::Get().GetTileById(MiscObject::JETPACK_UNIT);
        for (int i = 0; i < jetpackFuel; i++)
        {
            dst = {74 + 2 * i, 189, 6, 4};
            SDL_SetTextureBlendMode(texture.get(), SDL_BLENDMODE_BLEND);
            SDL_RenderCopy(renderer.get(), texture.get(), NULL, &dst);
        }
    }

    // Display gun if accquired
    if (gotGun)
    {
        texture = TileManager::Get().GetTileById(MiscObject::TEXT_GUN);
        dst = {62 + 8 + 185, 185, 62, 11};
        SDL_SetTextureBlendMode(texture.get(), SDL_BLENDMODE_BLEND);
        SDL_RenderCopy(renderer.get(), texture.get(), NULL, &dst);

        texture = TileManager::Get().GetTileById(StaticObject::GUN);
        dst = {62 + 8 + 185 + 62 + 8, 185, 62, 11};
        SDL_SetTextureBlendMode(texture.get(), SDL_BLENDMODE_BLEND);
        SDL_RenderCopy(renderer.get(), texture.get(), NULL, &dst);
    }

    // Display Go thru the door text
    if (gotTrophy)
    {
        texture = TileManager::Get().GetTileById(MiscObject::TEXT_CANEXIT);
        dst = {62 + 8, 185 + 16, 176, 14};
        SDL_SetTextureBlendMode(texture.get(), SDL_BLENDMODE_BLEND);
        SDL_RenderCopy(renderer.get(), texture.get(), NULL, &dst);
    }
}

void GameState::SetGotTrophy(const bool &status)
{
    this->gotTrophy = status;
}

void GameState::SetGotJetpack(const bool &status)
{
    this->gotJetpack = status;
}

void GameState::SetGotGun(const bool &status)
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
    --(this->lives);
}

void GameState::NextLevel()
{
    this->currentLevel = std::min(9, this->currentLevel + 1);
}

void GameState::SetPlayerX(const int &x)
{
    this->player->SetPlayerX(x);
}

void GameState::SetPlayerY(const int &y)
{
    this->player->SetPlayerY(y);
}

void GameState::SetPlayerPos(const int &x, const int &y)
{
    this->player->SetPlayerPos(x, y);
}

void GameState::Update()
{
    ++(this->ticks);
}

unsigned int GameState::GetTicks()
{
    return this->ticks;
}

GameState::~GameState()
{
}
