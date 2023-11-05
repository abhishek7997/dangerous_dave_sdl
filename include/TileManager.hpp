#pragma once

#include <iostream>
#include <vector>
#include <string>
#include <SDL_image.h>

class TileManager
{
private:
    void CopyTexture(SDL_Renderer *renderer, SDL_Texture *source, Uint32 &pixelFormat, const int &x, const int &y, const int &w, const int &h, int &tile);
    void LoadGameObjects(SDL_Renderer *renderer, SDL_Texture *source, int &tile, Uint32 pixelFormat);
    void LoadPlayerObjects(SDL_Renderer *renderer, SDL_Texture *source, int &tile, Uint32 pixelFormat);
    void LoadMonsterObjects(SDL_Renderer *renderer, SDL_Texture *source, int &tile, Uint32 pixelFormat);
    void LoadMiscObjects(SDL_Renderer *renderer, SDL_Texture *source, int &tile, Uint32 pixelFormat);

protected:
    TileManager(SDL_Renderer *renderer);
    static TileManager *instance;

public:
    static TileManager *getInstance(SDL_Renderer *renderer);
    static TileManager *getInstance();
    void LoadTiles();
    SDL_Texture *GetTileById(const int &id);
    ~TileManager();

private:
    const std::string m_TilemapPath = "D:\\Dev\\GameProgramming\\dangerous_dave_sdl\\assets\\dd_tilesheet.bmp";
    const int m_TilemapWidth = 145;
    const int m_TilemapHeight = 110;
    SDL_Renderer *renderer;
    std::vector<SDL_Texture *> tiles;
};