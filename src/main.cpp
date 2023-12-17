#include <iostream>
#include <SDL.h>
#include "SDLApp.hpp"
#include "GameState.hpp"
#include "TileManager.hpp"
#include "LevelManager.hpp"
#include "Level.hpp"

SDLApp &app = SDLApp::Get();
TileManager &tileManager = TileManager::Get();
LevelManager &levelManager = LevelManager::Get();
GameState &gameState = GameState::Get();

void HandleRendering()
{
    gameState.RenderStates();
    levelManager.GetCurrentLevel()->RenderLevel();
    gameState.GetPlayer()->RenderBullet(levelManager.GetCurrentOffset());
    gameState.GetPlayer()->Render(levelManager.GetCurrentOffset());
    EnemyBullet::RenderBullets();
}

void HandleUpdate()
{
    gameState.Update();
    Level *level = levelManager.GetCurrentLevel();
    if (!gameState.GetPlayer()->IsGrounded())
    {
        gameState.ApplyGravity();
    }
    if (gameState.GetPlayer()->GetDirection() == DIR::RIGHT)
    {
        if (gameState.GetPlayer()->GetRectangle().x > SCREENOFFSET::FIVE)
        {
            level->SetOffset(SCREENOFFSET::FIVE);
        }
        else if (gameState.GetPlayer()->GetRectangle().x > SCREENOFFSET::FOUR)
        {
            level->SetOffset(SCREENOFFSET::FOUR);
        }
        else if (gameState.GetPlayer()->GetRectangle().x > SCREENOFFSET::THREE)
        {
            level->SetOffset(SCREENOFFSET::THREE);
        }
        else if (gameState.GetPlayer()->GetRectangle().x > SCREENOFFSET::TWO)
        {
            level->SetOffset(SCREENOFFSET::TWO);
        }
        else if (gameState.GetPlayer()->GetRectangle().x > SCREENOFFSET::ONE)
        {
            level->SetOffset(SCREENOFFSET::ONE);
        }
    }
    else if (gameState.GetPlayer()->GetDirection() == DIR::LEFT)
    {
        if (gameState.GetPlayer()->GetRectangle().x < SCREENOFFSET::ONE)
        {
            level->SetOffset(SCREENOFFSET::ZERO);
        }
        else if (gameState.GetPlayer()->GetRectangle().x < SCREENOFFSET::TWO)
        {
            level->SetOffset(SCREENOFFSET::ONE);
        }
        else if (gameState.GetPlayer()->GetRectangle().x < SCREENOFFSET::THREE)
        {
            level->SetOffset(SCREENOFFSET::TWO);
        }
        else if (gameState.GetPlayer()->GetRectangle().x < SCREENOFFSET::FOUR)
        {
            level->SetOffset(SCREENOFFSET::THREE);
        }
        else if (gameState.GetPlayer()->GetRectangle().x < SCREENOFFSET::FIVE)
        {
            level->SetOffset(SCREENOFFSET::FOUR);
        }
    }
    gameState.GetPlayer()->UpdateFrame();
}

void HandleEvents()
{
    Level *level = levelManager.GetCurrentLevel();
    gameState.GetPlayer()->GetJumpTime();
    const Uint8 *keystates = SDL_GetKeyboardState(NULL);

    gameState.GetPlayer()->IsColliding();

    if (keystates[SDL_SCANCODE_RIGHT] && keystates[SDL_SCANCODE_UP])
    {
        gameState.GetPlayer()->MoveRight();
        gameState.GetPlayer()->Jump();
        gameState.GetPlayer()->SetTileId(PlayerObject::PLAYER_JUMP_R);
    }
    else if (keystates[SDL_SCANCODE_LEFT] && keystates[SDL_SCANCODE_UP])
    {
        gameState.GetPlayer()->MoveLeft();
        gameState.GetPlayer()->Jump();
        gameState.GetPlayer()->SetTileId(PlayerObject::PLAYER_JUMP_L);
    }
    else if (keystates[SDL_SCANCODE_RIGHT])
    {
        gameState.GetPlayer()->MoveRight();
        if (gameState.IsJetpackActivated())
        {
            gameState.GetPlayer()->SetTileId(PlayerObject::PLAYER_JETPACK_R_1);
        }
        else
        {
            gameState.GetPlayer()->SetTileId(PlayerObject::PLAYER_WALK_R_1);
        }
    }
    else if (keystates[SDL_SCANCODE_LEFT])
    {
        gameState.GetPlayer()->MoveLeft();
        if (gameState.IsJetpackActivated())
        {
            gameState.GetPlayer()->SetTileId(PlayerObject::PLAYER_JETPACK_L_1);
        }
        else
        {
            gameState.GetPlayer()->SetTileId(PlayerObject::PLAYER_WALK_L_1);
        }
    }
    else if (keystates[SDL_SCANCODE_UP])
    {
        if (gameState.IsJetpackActivated())
        {
            gameState.GetPlayer()->MoveUp();
        }
        gameState.GetPlayer()->Jump();
        gameState.GetPlayer()->SetTileId(PlayerObject::PLAYER_JUMP_R);
    }
    else if (keystates[SDL_SCANCODE_DOWN])
    {
        if (gameState.IsJetpackActivated())
        {
            gameState.GetPlayer()->MoveDown();
        }
    }

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            app.Stop();
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.scancode)
            {
            case SDL_SCANCODE_ESCAPE:
                app.Stop();
                break;
            case SDL_SCANCODE_LALT:
            case SDL_SCANCODE_RALT:
                if (gameState.GotJetpack())
                {
                    gameState.ToggleJetpack();
                    if (gameState.IsJetpackActivated())
                        gameState.GetPlayer()->SetTileId(PlayerObject::PLAYER_JETPACK_L_1);
                    else
                        gameState.GetPlayer()->SetTileId(PlayerObject::PLAYER_FRONT);
                }
                break;
            case SDL_SCANCODE_LCTRL:
            case SDL_SCANCODE_RCTRL:
                gameState.GetPlayer()->FireBullet();
                break;
            }
        }
    }
}

int main(int argc, char *argv[])
{
    app.SetEventCallback(HandleEvents);
    app.SetUpdateCallback(HandleUpdate);
    app.SetRenderCallback(HandleRendering);
    app.Run();

    return 0;
}