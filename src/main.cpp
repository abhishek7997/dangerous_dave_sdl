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

int k = 0;

void HandleUpdate()
{
    int x = 0; // rand() % levelData->getLevelHeight();
    int y = 0; // rand() % levelData->getLevelWidth();
    int id = k;
    // std::cout << "ID: " << id << std::endl;
    levelData->SetLevelDataTile(x, y, id);
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
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
            case SDLK_RIGHT:
                k++;
                if (k > 157)
                    k = 0;
                break;
            case SDLK_LEFT:
                k--;
                if (k < 0)
                    k = 157;
                break;
            default:
                break;
            }
            // std::cout << "Current tile id: " << k << std::endl;
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