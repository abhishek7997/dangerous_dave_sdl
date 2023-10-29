#include <iostream>
#include <SDL.h>
#include <Tile.hpp>

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
    TileData *tileData = new TileData();

    SDL_Surface *tilemap = tileData->LoadTilemap();
    SDL_Texture *tilemaptexture = SDL_CreateTextureFromSurface(renderer, tilemap);
    SDL_Rect rect;
    rect.x = 10;
    rect.y = 10;
    rect.w = 110;
    rect.h = 145;

    SDL_Event event;
    while (true)
    {
        SDL_RenderCopy(renderer, tilemaptexture, NULL, &rect);
        SDL_RenderDrawRect(renderer, &rect);
        SDL_RenderPresent(renderer);
        if (SDL_PollEvent(&event) && event.type == SDL_QUIT)
            break;
    }

    // SDL_Delay(5000);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}