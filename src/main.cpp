#include <iostream>
#include <SDL.h>
#include "TileManager.hpp"
#include "Level.hpp"
#include "LevelManager.hpp"
#include "GameState.hpp"
#include "SDLApp.hpp"

SDLApp *app;
TileManager *tileManager;
LevelManager *levelManager;
GameState *gameState;

void HandleRendering()
{
    gameState->RenderStates();
    levelManager->getLevel(gameState->getCurrentLevel())->RenderLevel();
    gameState->GetPlayer()->Render(app->GetRenderer(), levelManager->getLevel(gameState->getCurrentLevel())->GetOffset());
}

void HandleUpdate()
{
    Level *level = levelManager->getLevel(gameState->getCurrentLevel());
    if (gameState->GetPlayer()->IsGrounded(tileManager, level->GetLevel()) == SDL_FALSE)
    {
        gameState->applyGravity();
    }
    if (gameState->GetPlayer()->GetDirection() == DIR::RIGHT)
    {
        if (gameState->GetPlayer()->GetRectangle()->x > 5 * 20 * 16)
        {
            level->SetOffset(4 * 20 * 16);
        }
        else if (gameState->GetPlayer()->GetRectangle()->x > 4 * 20 * 16)
        {
            level->SetOffset(4 * 20 * 16);
        }
        else if (gameState->GetPlayer()->GetRectangle()->x > 3 * 20 * 16)
        {
            level->SetOffset(3 * 20 * 16);
        }
        else if (gameState->GetPlayer()->GetRectangle()->x > 2 * 20 * 16)
        {
            level->SetOffset(2 * 20 * 16);
        }
        else if (gameState->GetPlayer()->GetRectangle()->x > 20 * 16)
        {
            level->SetOffset(20 * 16);
        }
    }
    else if (gameState->GetPlayer()->GetDirection() == DIR::LEFT)
    {
        if (gameState->GetPlayer()->GetRectangle()->x < 20 * 16)
        {
            level->SetOffset(0);
        }
        else if (gameState->GetPlayer()->GetRectangle()->x < 2 * 20 * 16)
        {
            level->SetOffset(20 * 16);
        }
        else if (gameState->GetPlayer()->GetRectangle()->x < 3 * 20 * 16)
        {
            level->SetOffset(2 * 20 * 16);
        }
        else if (gameState->GetPlayer()->GetRectangle()->x < 4 * 20 * 16)
        {
            level->SetOffset(3 * 20 * 16);
        }
        else if (gameState->GetPlayer()->GetRectangle()->x < 5 * 20 * 16)
        {
            level->SetOffset(4 * 20 * 16);
        }
    }
    std::cout << "Player X: " << gameState->GetPlayer()->GetRectangle()->x << " Y: " << gameState->GetPlayer()->GetRectangle()->y << std::endl;

    // If player collides with monsters, player dies, camera (level offset) is reset, player starting position is reset
    if (gameState->GetPlayer()->IsColliding(level->GetLevel()) == SDL_TRUE)
    {
        std::cout << "Collided with something" << std::endl;
    }
}

void HandleEvents()
{
    Level *level = levelManager->getLevel(gameState->getCurrentLevel());
    gameState->GetPlayer()
        ->GetJumpTime();
    const Uint8 *keystates = SDL_GetKeyboardState(NULL);
    if (keystates[SDL_SCANCODE_RIGHT] && keystates[SDL_SCANCODE_UP])
    {
        gameState->moveRight();
        if (gameState->GetPlayer()->IsGrounded(tileManager, level->GetLevel()) == SDL_TRUE)
        {
            gameState->jump();
        }
        gameState->GetPlayer()->SetTileId(PlayerObject::PLAYER_JUMP_R);
    }
    else if (keystates[SDL_SCANCODE_LEFT] && keystates[SDL_SCANCODE_UP])
    {
        gameState->moveLeft();
        if (gameState->GetPlayer()->IsGrounded(tileManager, level->GetLevel()) == SDL_TRUE)
        {
            gameState->jump();
        }
        gameState->GetPlayer()->SetTileId(PlayerObject::PLAYER_JUMP_L);
    }
    else if (keystates[SDL_SCANCODE_RIGHT])
    {
        gameState->moveRight();
        if (gameState->jetpackState())
        {
            gameState->GetPlayer()->SetTileId(PlayerObject::PLAYER_JETPACK_R_1);
        }
        else
        {
            gameState->GetPlayer()->SetTileId(PlayerObject::PLAYER_WALK_R_1);
        }
    }
    else if (keystates[SDL_SCANCODE_LEFT])
    {
        gameState->moveLeft();
        if (gameState->jetpackState())
        {
            gameState->GetPlayer()->SetTileId(PlayerObject::PLAYER_JETPACK_L_1);
        }
        else
        {
            gameState->GetPlayer()->SetTileId(PlayerObject::PLAYER_WALK_L_1);
        }
    }
    else if (keystates[SDL_SCANCODE_UP])
    {
        if (gameState->jetpackState())
        {
            gameState->moveUp();
        }
        else if (gameState->GetPlayer()->IsGrounded(tileManager, level->GetLevel()) == SDL_TRUE)
        {
            gameState->jump();
            gameState->GetPlayer()->SetTileId(PlayerObject::PLAYER_JUMP_R);
        }
    }
    else if (keystates[SDL_SCANCODE_DOWN])
    {
        if (gameState->jetpackState())
        {
            gameState->moveDown();
        }
    }

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            app->Stop();
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.scancode)
            {
            case SDL_SCANCODE_LALT:
            case SDL_SCANCODE_RALT:
                gameState->toggleJetpack();
                if (gameState->jetpackState())
                    gameState->GetPlayer()->SetTileId(PlayerObject::PLAYER_JETPACK_L_1);
                else
                    gameState->GetPlayer()->SetTileId(PlayerObject::PLAYER_FRONT);
                break;
            }
        }
    }
}

int main(int argc, char *argv[])
{
    app = new SDLApp();
    gameState = GameState::getInstance(app->GetRenderer());
    tileManager = TileManager::getInstance(app->GetRenderer()); // Loaded the tile manager
    tileManager->LoadTiles();

    levelManager = LevelManager::getInstance();
    levelManager->SetRenderer(app->GetRenderer());
    levelManager->Initialize();

    std::cout << "Initialized level manager" << std::endl;

    app->SetEventCallback(HandleEvents);
    app->SetUpdateCallback(HandleUpdate);
    app->SetRenderCallback(HandleRendering);
    app->Run();

    delete levelManager;
    delete tileManager;
    delete app;

    return 0;
}