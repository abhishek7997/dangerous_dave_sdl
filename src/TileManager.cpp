
#include <SDL.h>
#include "TileManager.hpp"

void TileManager::CopyTexture(SDL_Renderer *renderer, SDL_Texture *source, Uint32 &pixelFormat, const int &x, const int &y, const int &w, const int &h, int &tile)
{
    SDL_Rect rect;
    rect.x = x;
    rect.y = y;
    rect.w = w;
    rect.h = h;

    tiles[tile] = SDL_CreateTexture(renderer, pixelFormat, SDL_TEXTUREACCESS_TARGET, w, h);
    SDL_SetTextureBlendMode(tiles[tile], SDL_BLENDMODE_BLEND);
    SDL_SetRenderTarget(renderer, tiles[tile++]);
    SDL_RenderCopy(renderer, source, &rect, NULL);
}

void TileManager::LoadGameObjects(SDL_Renderer *renderer, SDL_Texture *source, int &tile, Uint32 pixelFormat)
{
    const int tileWidth = 16;
    const int tileHeight = 16;
    SDL_Rect rect;
    int i = 0;
    int j = 0;

    for (i = 0; i < 6; i++)
    {
        for (j = 0; j < 8; j++)
        {
            CopyTexture(renderer, source, pixelFormat, j * tileWidth, i * tileHeight, tileWidth, tileHeight, tile);
        }
    }

    // Load remaining game objects
    i = 6;
    for (j = 0; j < 5; j++)
    {
        rect.x = j * tileWidth;
        rect.y = i * tileHeight;
        rect.w = tileWidth;
        rect.h = tileHeight;

        CopyTexture(renderer, source, pixelFormat, j * tileWidth, i * tileHeight, tileWidth, tileHeight, tile);
    }
    std::cout << "Tile count for LoadGameObjects: " << tile << std::endl;
}

void TileManager::LoadPlayerObjects(SDL_Renderer *renderer, SDL_Texture *source, int &tile, Uint32 pixelFormat)
{
    int tileWidth = 20;
    int tileHeight = 16;
    int startX = 80;
    int startY = 96;
    SDL_Rect rect;
    int i = 0;
    int j = 0;

    // Load some player objects
    for (j = 0; j < 3; j++)
    {
        CopyTexture(renderer, source, pixelFormat, startX + j * tileWidth, startY, tileWidth, tileHeight, tile);
    }

    // Load remaining player objects
    startX = 0;
    startY = 112;
    for (i = 0; i < 4; i++)
    {
        for (j = 0; j < 8; j++)
        {
            CopyTexture(renderer, source, pixelFormat, startX + j * tileWidth, startY + i * tileHeight, tileWidth, tileHeight, tile);
        }
    }

    startX = 0;
    startY = 176;
    // Load last player object
    CopyTexture(renderer, source, pixelFormat, startX, startY, tileWidth, tileHeight, tile);
    std::cout << "Tile count for LoadPlayerObjects: " << tile << std::endl;
}

void TileManager::LoadMonsterObjects(SDL_Renderer *renderer, SDL_Texture *source, int &tile, Uint32 pixelFormat)
{
    int tileWidth = 28;
    int tileHeight = 15;
    int startX = 20;
    int startY = 181;
    SDL_Rect rect;
    int j;

    // Load monster objects (spider)
    for (j = 0; j < 4; j++)
    {
        CopyTexture(renderer, source, pixelFormat, startX + j * tileWidth, startY, tileWidth, tileHeight, tile);
    }

    // Load monster objects (Blade; first 3)
    tileWidth = 24;
    tileHeight = 20;
    startX = 132;
    startY = 176;
    for (j = 0; j < 3; j++)
    {
        CopyTexture(renderer, source, pixelFormat, startX + j * tileWidth, startY, tileWidth, tileHeight, tile);
    }

    startX = 0;
    startY = 196;
    CopyTexture(renderer, source, pixelFormat, startX, startY, tileWidth, tileHeight, tile);

    // Load monster objects (Red sun; all 4)
    tileWidth = 24;
    tileHeight = 21;
    startX = 24;
    startY = 196;
    for (j = 0; j < 4; j++)
    {
        CopyTexture(renderer, source, pixelFormat, startX + j * tileWidth, startY, tileWidth, tileHeight, tile);
    }

    // Load monster objects (Green stick; first 3)
    tileWidth = 24;
    tileHeight = 22;
    startX = 120;
    startY = 196;
    for (j = 0; j < 3; j++)
    {
        CopyTexture(renderer, source, pixelFormat, startX + j * tileWidth, startY, tileWidth, tileHeight, tile);
    }

    // Load last monster object (green stick)
    startX = 0;
    startY = 218;
    CopyTexture(renderer, source, pixelFormat, startX, startY, tileWidth, tileHeight, tile);

    // Load monster objects (UFO; all 4)
    tileWidth = 18;
    tileHeight = 8;
    startX = 24;
    startY = 218;
    for (j = 0; j < 4; j++)
    {
        CopyTexture(renderer, source, pixelFormat, startX + j * tileWidth, startY, tileWidth, tileHeight, tile);
    }

    // Load monster objects (Brown thing; first 3)
    tileWidth = 16;
    tileHeight = 12;
    startX = 96;
    startY = 218;
    for (j = 0; j < 3; j++)
    {
        CopyTexture(renderer, source, pixelFormat, startX + j * tileWidth, startY, tileWidth, tileHeight, tile);
    }

    // Load last monster object (brown thing)
    startX = 0;
    startY = 240;
    CopyTexture(renderer, source, pixelFormat, startX, startY, tileWidth, tileHeight, tile);

    // Load monster objects (Green balls; all 4)
    tileWidth = 16;
    tileHeight = 16;
    startX = 16;
    startY = 240;
    for (j = 0; j < 4; j++)
    {
        CopyTexture(renderer, source, pixelFormat, startX + j * tileWidth, startY, tileWidth, tileHeight, tile);
    }

    // Load monster objects (vertical disc; first 3)
    tileWidth = 16;
    tileHeight = 20;
    startX = 80;
    startY = 240;
    for (j = 0; j < 3; j++)
    {
        CopyTexture(renderer, source, pixelFormat, startX + j * tileWidth, startY, tileWidth, tileHeight, tile);
    }

    // Load last monster object (vertical disc)
    startX = 0;
    startY = 260;
    CopyTexture(renderer, source, pixelFormat, startX, startY, tileWidth, tileHeight, tile);

    // Load three enemy bullets (right)
    tileWidth = 20;
    tileHeight = 3;
    startX = 16;
    startY = 260;
    for (j = 0; j < 3; j++)
    {
        CopyTexture(renderer, source, pixelFormat, startX + j * tileWidth, startY, tileWidth, tileHeight, tile);
    }

    // Load three enemy bullets (left)
    startX = 75;
    startY = 260;
    for (j = 0; j < 3; j++)
    {
        CopyTexture(renderer, source, pixelFormat, startX + j * tileWidth, startY, tileWidth, tileHeight, tile);
    }
    std::cout << "Tile count for LoadMonsterObjects: " << tile << std::endl;
}

void TileManager::LoadMiscObjects(SDL_Renderer *renderer, SDL_Texture *source, int &tile, Uint32 pixelFormat)
{
    // Load player bullet (right)
    int tileWidth = 12;
    int tileHeight = 3;
    int startX = 136;
    int startY = 260;

    CopyTexture(renderer, source, pixelFormat, startX, startY, tileWidth, tileHeight, tile);

    // Load player bullet (left)
    startX = 0;
    startY = 280;

    CopyTexture(renderer, source, pixelFormat, startX, startY, tileWidth, tileHeight, tile);

    // Load death animations
    tileWidth = 20;
    tileHeight = 13;
    startX = 12;
    startY = 280;
    for (int j = 0; j < 4; j++)
    {
        CopyTexture(renderer, source, pixelFormat, startX + j * tileWidth, startY, tileWidth, tileHeight, tile);
    }

    // JETPACK text
    tileWidth = 62;
    tileHeight = 11;
    startX = 92;
    startY = 280;
    CopyTexture(renderer, source, pixelFormat, startX, startY, tileWidth, tileHeight, tile);

    // GUN Text
    tileWidth = 62;
    tileHeight = 11;
    startX = 154;
    startY = 280;
    CopyTexture(renderer, source, pixelFormat, startX, startY, tileWidth, tileHeight, tile);

    // DAVES:___
    tileWidth = 62;
    tileHeight = 11;
    startX = 216;
    startY = 280;
    CopyTexture(renderer, source, pixelFormat, startX, startY, tileWidth, tileHeight, tile);

    // LEVEL:___
    tileWidth = 62;
    tileHeight = 11;
    startX = 0;
    startY = 293;
    CopyTexture(renderer, source, pixelFormat, startX, startY, tileWidth, tileHeight, tile);

    // SCORE:___
    tileWidth = 62;
    tileHeight = 11;
    startX = 62;
    startY = 293;
    CopyTexture(renderer, source, pixelFormat, startX, startY, tileWidth, tileHeight, tile);

    // GO THRU THE DOOR
    // LEVEL:___
    tileWidth = 176;
    tileHeight = 14;
    startX = 124;
    startY = 293;
    CopyTexture(renderer, source, pixelFormat, startX, startY, tileWidth, tileHeight, tile);

    // WARP Text
    tileWidth = 73;
    tileHeight = 15;
    startX = 300;
    startY = 293;
    CopyTexture(renderer, source, pixelFormat, startX, startY, tileWidth, tileHeight, tile);

    // ZONE Text
    tileWidth = 67;
    tileHeight = 15;
    startX = 373;
    startY = 293;
    CopyTexture(renderer, source, pixelFormat, startX, startY, tileWidth, tileHeight, tile);

    // JETPACK meter
    tileWidth = 130;
    tileHeight = 12;
    startX = 440;
    startY = 293;
    CopyTexture(renderer, source, pixelFormat, startX, startY, tileWidth, tileHeight, tile);

    // JETPACK unit
    tileWidth = 6;
    tileHeight = 4;
    startX = 570;
    startY = 293;
    CopyTexture(renderer, source, pixelFormat, startX, startY, tileWidth, tileHeight, tile);

    // DAVE (Lives) unit
    tileWidth = 17;
    tileHeight = 12;
    startX = 576;
    startY = 293;
    CopyTexture(renderer, source, pixelFormat, startX, startY, tileWidth, tileHeight, tile);

    // Banners (4)
    tileWidth = 112;
    tileHeight = 47;
    startX = 0;
    startY = 308;
    for (int j = 0; j < 4; j++)
    {
        CopyTexture(renderer, source, pixelFormat, startX + j * tileWidth, startY, tileWidth, tileHeight, tile);
    }

    // Digits (0 to 3)
    tileWidth = 8;
    tileHeight = 11;
    startX = 448;
    startY = 308;
    for (int j = 0; j < 4; j++)
    {
        CopyTexture(renderer, source, pixelFormat, startX + j * tileWidth, startY, tileWidth, tileHeight, tile);
    }

    // Digits (4 to 9)
    tileWidth = 8;
    tileHeight = 11;
    startX = 0;
    startY = 355;
    for (int j = 0; j < 6; j++)
    {
        CopyTexture(renderer, source, pixelFormat, startX + j * tileWidth, startY, tileWidth, tileHeight, tile);
    }

    std::cout << "Tile count for LoadMiscObjects: " << tile << std::endl;
}

TileManager::TileManager(SDL_Renderer *renderer)
{
    if (IMG_Init(IMG_INIT_PNG) < 0)
    {
        std::cout << "Could not initialize SDL_Image: " << SDL_GetError() << std::endl;
    }

    this->tiles = std::vector<SDL_Texture *>(158);
    this->renderer = renderer;
}

TileManager *TileManager::getInstance(SDL_Renderer *renderer)
{
    if (instance == nullptr)
    {
        instance = new TileManager(renderer);
    }
    return instance;
}

TileManager *TileManager::getInstance()
{
    return instance;
}

void TileManager::LoadTiles()
{
    if (!this->renderer)
    {
        SDL_Log("Renderer not set\n");
        return;
    }
    SDL_Surface *surface = SDL_LoadBMP(m_TilemapPath.c_str());
    if (!surface)
    {
        std::cout << "Tilemap not loaded" << std::endl;
        return;
    }

    SDL_PixelFormat *pixelFormat = surface->format;

    if (SDL_SetColorKey(surface, SDL_TRUE, SDL_MapRGB(pixelFormat, 0x00, 0x00, 0x00)) < 0)
    {
        std::cout << "Could not set color key: " << SDL_GetError() << std::endl;
    }

    SDL_Texture *m_TilemapTexture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FreeSurface(surface);

    if (!m_TilemapTexture)
    {
        std::cout << "Could not load texture: " << SDL_GetError() << std::endl;
        return;
    }

    int tile = 0;

    LoadGameObjects(renderer, m_TilemapTexture, tile, pixelFormat->format);
    LoadPlayerObjects(renderer, m_TilemapTexture, tile, pixelFormat->format);
    LoadMonsterObjects(renderer, m_TilemapTexture, tile, pixelFormat->format);
    LoadMiscObjects(renderer, m_TilemapTexture, tile, pixelFormat->format);

    SDL_SetRenderTarget(renderer, NULL);

    SDL_DestroyTexture(m_TilemapTexture);

    std::cout
        << "Final tile count: " << tile << std::endl;
}

SDL_Texture *TileManager::GetTileById(const int &id)
{
    return tiles[id];
}

TileManager::~TileManager()
{
}

TileManager *TileManager::instance = nullptr;