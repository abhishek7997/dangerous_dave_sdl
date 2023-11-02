#include <iostream>
#include <SDL.h>
#include "TileManager.hpp"
#include "Level.hpp"
#include "LevelData.hpp"
#include "SDLApp.hpp"

SDLApp *app;
// LevelData *levelData;
TileManager *tileManager;
Level *level;

void HandleRendering()
{
    level->RenderLevel(tileManager);
    // levelData->RenderLevelData(app->GetRenderer(), tileManager);
}

void HandleUpdate()
{
}

void HandleEvents()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            app->Stop();
            break;
        default:
            break;
        }
    }
}

int main(int argc, char *argv[])
{
    app = new SDLApp();
    // levelData = new LevelData();
    tileManager = new TileManager(app->GetRenderer());
    tileManager->LoadTiles();

    level = new Level(app->GetRenderer(), tileManager);
    level->CreateLevel();

    app->SetEventCallback(HandleEvents);
    app->SetUpdateCallback(HandleUpdate);
    app->SetRenderCallback(HandleRendering);
    app->Run();

    delete level;
    delete tileManager;
    // delete levelData;
    delete app;

    return 0;
}