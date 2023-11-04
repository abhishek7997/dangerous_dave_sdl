#include <iostream>
#include <SDL.h>
#include "TileManager.hpp"
#include "Level.hpp"
#include "LevelManager.hpp"
#include "SDLApp.hpp"

SDLApp *app;
TileManager *tileManager;
LevelManager *levelManager;
Level *level;

void HandleRendering()
{
    level->RenderLevel();
    app->GetGameState()->GetPlayer()->Render(app->GetRenderer(), tileManager, level->GetOffset());
}

void HandleUpdate()
{
    if (app->GetGameState()->GetPlayer()->IsGrounded(tileManager, level->GetLevel()) == SDL_FALSE)
    {
        app->GetGameState()->applyGravity();
    }
    if (app->GetGameState()->GetPlayer()->GetDirection() == DIR::RIGHT)
    {
        if (app->GetGameState()->GetPlayer()->GetRectangle()->x > 5 * 20 * 16)
        {
            level->SetOffset(4 * 20 * 16);
        }
        else if (app->GetGameState()->GetPlayer()->GetRectangle()->x > 4 * 20 * 16)
        {
            level->SetOffset(4 * 20 * 16);
        }
        else if (app->GetGameState()->GetPlayer()->GetRectangle()->x > 3 * 20 * 16)
        {
            level->SetOffset(3 * 20 * 16);
        }
        else if (app->GetGameState()->GetPlayer()->GetRectangle()->x > 2 * 20 * 16)
        {
            level->SetOffset(2 * 20 * 16);
        }
        else if (app->GetGameState()->GetPlayer()->GetRectangle()->x > 20 * 16)
        {
            level->SetOffset(20 * 16);
        }
    }
    else if (app->GetGameState()->GetPlayer()->GetDirection() == DIR::LEFT)
    {
        if (app->GetGameState()->GetPlayer()->GetRectangle()->x < 20 * 16)
        {
            level->SetOffset(0);
        }
        else if (app->GetGameState()->GetPlayer()->GetRectangle()->x < 2 * 20 * 16)
        {
            level->SetOffset(20 * 16);
        }
        else if (app->GetGameState()->GetPlayer()->GetRectangle()->x < 3 * 20 * 16)
        {
            level->SetOffset(2 * 20 * 16);
        }
        else if (app->GetGameState()->GetPlayer()->GetRectangle()->x < 4 * 20 * 16)
        {
            level->SetOffset(3 * 20 * 16);
        }
        else if (app->GetGameState()->GetPlayer()->GetRectangle()->x < 5 * 20 * 16)
        {
            level->SetOffset(4 * 20 * 16);
        }
    }
    std::cout << "Player X: " << app->GetGameState()->GetPlayer()->GetRectangle()->x << " Y: " << app->GetGameState()->GetPlayer()->GetRectangle()->y << std::endl;
}

void HandleEvents()
{
    app->GetGameState()->GetPlayer()->GetJumpTime();
    const Uint8 *keystates = SDL_GetKeyboardState(NULL);
    if (keystates[SDL_SCANCODE_RIGHT] && keystates[SDL_SCANCODE_UP])
    {
        app->GetGameState()->moveRight();
        if (app->GetGameState()->GetPlayer()->IsGrounded(tileManager, level->GetLevel()) == SDL_TRUE)
        {
            app->GetGameState()->jump();
            app->GetGameState()->GetPlayer()->SetTileId(PlayerObject::PLAYER_JUMP_R);
        }
    }
    else if (keystates[SDL_SCANCODE_LEFT] && keystates[SDL_SCANCODE_UP])
    {
        app->GetGameState()->moveLeft();
        if (app->GetGameState()->GetPlayer()->IsGrounded(tileManager, level->GetLevel()) == SDL_TRUE)
        {
            app->GetGameState()->jump();
            app->GetGameState()->GetPlayer()->SetTileId(PlayerObject::PLAYER_JUMP_L);
        }
    }
    else if (keystates[SDL_SCANCODE_RIGHT])
    {
        app->GetGameState()->moveRight();
        if (app->GetGameState()->jetpackState())
        {
            app->GetGameState()->GetPlayer()->SetTileId(PlayerObject::PLAYER_JETPACK_R_1);
        }
        else
        {
            app->GetGameState()->GetPlayer()->SetTileId(PlayerObject::PLAYER_WALK_R_1);
        }
    }
    else if (keystates[SDL_SCANCODE_LEFT])
    {
        app->GetGameState()->moveLeft();
        if (app->GetGameState()->jetpackState())
        {
            app->GetGameState()->GetPlayer()->SetTileId(PlayerObject::PLAYER_JETPACK_L_1);
        }
        else
        {
            app->GetGameState()->GetPlayer()->SetTileId(PlayerObject::PLAYER_WALK_L_1);
        }
    }
    else if (keystates[SDL_SCANCODE_UP])
    {
        if (app->GetGameState()->jetpackState())
        {
            app->GetGameState()->moveUp();
        }
        else if (app->GetGameState()->GetPlayer()->IsGrounded(tileManager, level->GetLevel()) == SDL_TRUE)
        {
            app->GetGameState()->jump();
            app->GetGameState()->GetPlayer()->SetTileId(PlayerObject::PLAYER_JUMP_R);
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
                app->GetGameState()->toggleJetpack();
                if (app->GetGameState()->jetpackState())
                    app->GetGameState()->GetPlayer()->SetTileId(PlayerObject::PLAYER_JETPACK_L_1);
                else
                    app->GetGameState()->GetPlayer()->SetTileId(PlayerObject::PLAYER_FRONT);
                break;
            }
        }
    }
}

int main(int argc, char *argv[])
{
    app = new SDLApp();
    tileManager = new TileManager(app->GetRenderer());
    tileManager->LoadTiles();

    levelManager = new LevelManager();

    level = new Level(app->GetRenderer(), tileManager);
    level->CreateLevel();

    app->SetEventCallback(HandleEvents);
    app->SetUpdateCallback(HandleUpdate);
    app->SetRenderCallback(HandleRendering);
    app->Run();

    delete level;
    delete levelManager;
    delete tileManager;
    delete app;

    return 0;
}