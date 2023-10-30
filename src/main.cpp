#include <iostream>
#include <SDL.h>
#include "Tile.hpp"
#include "Level.hpp"
#include "SDLApp.hpp"

SDLApp *app;
LevelData *levelData;
TileManager *tileManager;

void HandleRendering()
{
    levelData->RenderLevelData(app->GetRenderer(), tileManager);
}

void HandleUpdate()
{
    int k = 144;
    for (int i = 0; i < 20; i++)
    {
        int x = rand() % levelData->getLevelHeight();
        int y = rand() % levelData->getLevelWidth();
        int id = k;

        k++;
        if (k == 147)
            k = 144;

        levelData->SetLevelDataTile(x, y, id);
    }
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
    levelData = new LevelData();
    tileManager = new TileManager();
    tileManager->LoadTiles(app->GetRenderer());

    app->SetEventCallback(HandleEvents);
    app->SetUpdateCallback(HandleUpdate);
    app->SetRenderCallback(HandleRendering);
    app->Run();

    delete tileManager;
    delete levelData;
    delete app;

    return 0;
}