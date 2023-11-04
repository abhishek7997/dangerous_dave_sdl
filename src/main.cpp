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
    app->GetGameState()->GetPlayer()->Render(app->GetRenderer(), tileManager);
}

void HandleUpdate()
{
    if (app->GetGameState()->GetPlayer()->IsGrounded(tileManager, level->GetLevel()) == SDL_FALSE)
    {
        app->GetGameState()->applyGravity();
    }
}

void HandleEvents()
{
    app->GetGameState()->GetPlayer()->GetJumpTime();
    const Uint8 *keystates = SDL_GetKeyboardState(NULL);
    if (keystates[SDL_SCANCODE_RIGHT] && keystates[SDL_SCANCODE_UP])
    {
        std::cout << "Jump right" << std::endl;
        app->GetGameState()->moveRight();
        if (app->GetGameState()->GetPlayer()->IsGrounded(tileManager, level->GetLevel()) == SDL_TRUE)
        {
            app->GetGameState()->jump();
        }
    }
    else if (keystates[SDL_SCANCODE_LEFT] && keystates[SDL_SCANCODE_UP])
    {
        std::cout << "Jump left" << std::endl;
        app->GetGameState()->moveLeft();
        if (app->GetGameState()->GetPlayer()->IsGrounded(tileManager, level->GetLevel()) == SDL_TRUE)
        {
            app->GetGameState()->jump();
        }
    }
    else if (keystates[SDL_SCANCODE_RIGHT])
    {
        std::cout << "Move right" << std::endl;
        app->GetGameState()->moveRight();
    }
    else if (keystates[SDL_SCANCODE_LEFT])
    {
        std::cout << "Move left" << std::endl;
        app->GetGameState()->moveLeft();
    }
    else if (keystates[SDL_SCANCODE_UP])
    {
        if (app->GetGameState()->jetpackState())
        {
            app->GetGameState()->moveUp();
        }
        else if (app->GetGameState()->GetPlayer()->IsGrounded(tileManager, level->GetLevel()) == SDL_TRUE)
        {
            std::cout << "Jump up" << std::endl;
            app->GetGameState()->jump();
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
                std::cout << "Activate jetpack if fuel is there" << std::endl;
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

    levelManager = new LevelManager(tileManager);

    level = new Level(app->GetRenderer(), tileManager);
    level->CreateLevel();

    app->SetEventCallback(HandleEvents);
    app->SetUpdateCallback(HandleUpdate);
    app->SetRenderCallback(HandleRendering);
    app->Run();

    delete level;
    delete tileManager;
    delete app;

    return 0;
}