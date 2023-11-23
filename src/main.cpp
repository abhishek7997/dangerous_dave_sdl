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
    levelManager->GetCurrentLevel()->RenderLevel();
    gameState->GetPlayer()->RenderBullet(app->GetRenderer(), levelManager->GetCurrentOffset());
    gameState->GetPlayer()->Render(app->GetRenderer(), levelManager->GetCurrentOffset());
}

void HandleUpdate()
{
    Level *level = levelManager->GetCurrentLevel();
    if (!gameState->GetPlayer()->IsGrounded())
    {
        gameState->applyGravity();
    }
    if (gameState->GetPlayer()->GetDirection() == DIR::RIGHT)
    {
        if (gameState->GetPlayer()->GetRectangle()->x > SCREENOFFSET::FIVE)
        {
            level->SetOffset(SCREENOFFSET::FIVE);
        }
        else if (gameState->GetPlayer()->GetRectangle()->x > SCREENOFFSET::FOUR)
        {
            level->SetOffset(SCREENOFFSET::FOUR);
        }
        else if (gameState->GetPlayer()->GetRectangle()->x > SCREENOFFSET::THREE)
        {
            level->SetOffset(SCREENOFFSET::THREE);
        }
        else if (gameState->GetPlayer()->GetRectangle()->x > SCREENOFFSET::TWO)
        {
            level->SetOffset(SCREENOFFSET::TWO);
        }
        else if (gameState->GetPlayer()->GetRectangle()->x > SCREENOFFSET::ONE)
        {
            level->SetOffset(SCREENOFFSET::ONE);
        }
    }
    else if (gameState->GetPlayer()->GetDirection() == DIR::LEFT)
    {
        if (gameState->GetPlayer()->GetRectangle()->x < SCREENOFFSET::ONE)
        {
            level->SetOffset(SCREENOFFSET::ZERO);
        }
        else if (gameState->GetPlayer()->GetRectangle()->x < SCREENOFFSET::TWO)
        {
            level->SetOffset(SCREENOFFSET::ONE);
        }
        else if (gameState->GetPlayer()->GetRectangle()->x < SCREENOFFSET::THREE)
        {
            level->SetOffset(SCREENOFFSET::TWO);
        }
        else if (gameState->GetPlayer()->GetRectangle()->x < SCREENOFFSET::FOUR)
        {
            level->SetOffset(SCREENOFFSET::THREE);
        }
        else if (gameState->GetPlayer()->GetRectangle()->x < SCREENOFFSET::FIVE)
        {
            level->SetOffset(SCREENOFFSET::FOUR);
        }
    }
    gameState->GetPlayer()->UpdateFrame();
    // gameState->GetPlayer()->PrintRectCoordinates();
    // std::cout << "Player X: " << gameState->GetPlayer()->GetRectangle()->x << " Y: " << gameState->GetPlayer()->GetRectangle()->y << std::endl;
}

void HandleEvents()
{
    Level *level = levelManager->getLevel(gameState->getCurrentLevel());
    gameState->GetPlayer()->GetJumpTime();
    const Uint8 *keystates = SDL_GetKeyboardState(NULL);

    gameState->GetPlayer()->IsColliding();

    if (keystates[SDL_SCANCODE_RIGHT] && keystates[SDL_SCANCODE_UP])
    {
        if (gameState->GetPlayer()->canMoveRight())
        {
            gameState->moveRight();
            gameState->jump();
            gameState->GetPlayer()->SetTileId(PlayerObject::PLAYER_JUMP_R);
        }
    }
    else if (keystates[SDL_SCANCODE_LEFT] && keystates[SDL_SCANCODE_UP])
    {
        if (gameState->GetPlayer()->canMoveLeft())
        {
            gameState->moveLeft();
            gameState->jump();
            gameState->GetPlayer()->SetTileId(PlayerObject::PLAYER_JUMP_L);
        }
    }
    else if (keystates[SDL_SCANCODE_RIGHT])
    {
        if (gameState->GetPlayer()->canMoveRight())
        {
            gameState->moveRight();
        }
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
        if (gameState->GetPlayer()->canMoveLeft())
        {
            gameState->moveLeft();
        }
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
            if (gameState->GetPlayer()->canMoveUp())
            {
                gameState->moveUp();
            }
        }
        else if (gameState->GetPlayer()->IsGrounded())
        {
            gameState->jump();
            gameState->GetPlayer()->SetTileId(PlayerObject::PLAYER_JUMP_R);
        }
    }
    else if (keystates[SDL_SCANCODE_DOWN])
    {
        if (gameState->jetpackState())
        {
            if (gameState->GetPlayer()->canMoveDown())
            {
                gameState->moveDown();
            }
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
                if (gameState->GotJetpack())
                {
                    gameState->toggleJetpack();
                    if (gameState->jetpackState())
                        gameState->GetPlayer()->SetTileId(PlayerObject::PLAYER_JETPACK_L_1);
                    else
                        gameState->GetPlayer()->SetTileId(PlayerObject::PLAYER_FRONT);
                }
                break;
            case SDL_SCANCODE_LCTRL:
            case SDL_SCANCODE_RCTRL:
                gameState->GetPlayer()->FireBullet();
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