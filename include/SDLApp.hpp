#include <iostream>
#include <functional>
#include <SDL.h>
#include <SDL_image.h>
#include "GameState.hpp"

class SDLApp
{
public:
    SDLApp()
    {
        std::cout << "Initializing system" << std::endl;
        if (Initialize() < 0)
            std::cout << "Could not initialize SDLApp" << std::endl;
    }
    int Initialize()
    {
        if (SDL_Init(SDL_INIT_VIDEO) < 0)
        {
            std::cout << "Could not initialize SDL: " << SDL_GetError() << std::endl;
            return -1;
        }
        if (IMG_Init(IMG_INIT_PNG) < 0)
        {
            std::cout << "Could not initialize SDL_Image: " << SDL_GetError() << std::endl;
            return -1;
        }

        gameState = new GameState();

        window = SDL_CreateWindow("DD_SDL", 20, 20, m_WindowWidth, m_WindowHeight, SDL_WINDOW_SHOWN);
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

        SDL_RenderSetScale(renderer, 3, 3);
        return 0;
    }

    void SetEventCallback(std::function<void(void)> func)
    {
        m_EventCallback = func;
    }

    void SetUpdateCallback(std::function<void(void)> func)
    {
        m_UpdateCallback = func;
    }

    void SetRenderCallback(std::function<void(void)> func)
    {
        m_RenderCallback = func;
    }
    void Run()
    {
        SDL_Event event;
        Uint32 starttime, endtime, deltatime;
        while (isRunning)
        {
            starttime = SDL_GetTicks();

            SDL_SetRenderDrawColor(renderer, 0x0, 0x0, 0x0, SDL_ALPHA_OPAQUE);
            SDL_RenderClear(renderer);
            m_EventCallback();
            m_UpdateCallback();
            m_RenderCallback();
            SDL_RenderPresent(renderer);

            endtime = SDL_GetTicks();

            deltatime = endtime - starttime;
            if (deltatime <= (1000 / m_MaxFrameRate))
                SDL_Delay((1000 / m_MaxFrameRate) - deltatime);
        }
    }

    void Stop()
    {
        isRunning = false;
    }

    SDL_Renderer *GetRenderer()
    {
        return renderer;
    }

    ~SDLApp()
    {
        delete gameState;
        IMG_Quit();
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    SDL_bool isColliding(SDL_Rect *a, SDL_Rect *b)
    {
        return SDL_HasIntersection(a, b);
    }

    GameState *GetGameState()
    {
        return this->gameState;
    }

private:
    bool isRunning = true;
    const int m_WindowWidth = 960;
    const int m_WindowHeight = 480;
    const int m_MaxFrameRate = 30;
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    std::function<void(void)> m_EventCallback = []() {};
    std::function<void(void)> m_UpdateCallback = []() {};
    std::function<void(void)> m_RenderCallback = []() {};
    GameState *gameState = nullptr;
};