#include <iostream>
#include <SDL.h>
#include "Tile.hpp"
#include "Level.hpp"

#define WIDTH 640
#define HEIGHT 480

int main(int argc, char *argv[])
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cout << "Could not initialize SDL: " << SDL_GetError() << std::endl;
        return -1;
    }
    SDL_Window *window = SDL_CreateWindow("DD_SDL", 20, 20, WIDTH, HEIGHT, SDL_WINDOW_SHOWN);

    if (!window)
    {
        std::cout << "Cound not create window: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    std::cout << "Before loading TileData" << std::endl;
    TileData *tileData = new TileData(renderer);
    std::cout << "After loading TileData" << std::endl;
    LevelData *levelData = new LevelData();
    std::cout << "After declaring LeveLData" << std::endl;
    std::cout << "Before calling LoadTiles()" << std::endl;

    tileData->LoadTiles();
    std::cout << "After calling LoadTiles()" << std::endl;
    // SDL_Texture *tilemaptexture = tileData->GetTilemapTexture();
    // SDL_Rect rect;
    // rect.x = 10;
    // rect.y = 10;
    // rect.w = 110;
    // rect.h = 145;

    SDL_Event event;
    while (true)
    {
        // SDL_RenderCopy(renderer, tilemaptexture, NULL, &rect);
        // SDL_RenderDrawRect(renderer, &rect);
        levelData->RenderLevelData(renderer, tileData);
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
            break;
        SDL_RenderPresent(renderer);
    }

    // SDL_Delay(5000);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}