#include <iostream>
#include <SDL.h>
#include "Tile.hpp"
#include "Level.hpp"
#include "SDLApp.hpp"

SDLApp *app;
LevelData *levelData;
TileData *tileData;

void HandleRendering()
{
    levelData->RenderLevelData(app->GetRenderer(), tileData);
}

void HandleUpdate()
{
    for (int i = 0; i < 3; i++)
    {
        int x = rand() % 26;
        int y = rand() % 35;
        int id = rand() % 48;

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
    tileData = new TileData(app->GetRenderer());
    tileData->LoadTiles();

    app->SetEventCallback(HandleEvents);
    app->SetUpdateCallback(HandleUpdate);
    app->SetRenderCallback(HandleRendering);
    app->Run();

    delete tileData;
    delete levelData;
    delete app;

    return 0;
}