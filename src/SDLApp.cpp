#include "SDLApp.hpp"

SDLApp::SDLApp()
{
    std::cout << "Initializing system" << std::endl;
    if (Initialize() < 0)
        std::cout << "Could not initialize SDLApp" << std::endl;
}

SDLApp *SDLApp::getInstance()
{
    if (instance == nullptr)
    {
        instance = new SDLApp();
    }
    return instance;
}

int SDLApp::Initialize()
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

    window = SDL_CreateWindow("DD_SDL", 20, 20, m_WindowWidth, m_WindowHeight, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    SDL_RenderSetScale(renderer, 3, 3);
    return 0;
}

void SDLApp::SetEventCallback(std::function<void(void)> func)
{
    m_EventCallback = func;
}

void SDLApp::SetUpdateCallback(std::function<void(void)> func)
{
    m_UpdateCallback = func;
}

void SDLApp::SetRenderCallback(std::function<void(void)> func)
{
    m_RenderCallback = func;
}

void SDLApp::Run()
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

void SDLApp::Stop()
{
    this->isRunning = false;
}

SDL_Renderer *SDLApp::GetRenderer()
{
    return this->renderer;
}

SDLApp::~SDLApp()
{
    IMG_Quit();
    SDL_DestroyRenderer(this->renderer);
    SDL_DestroyWindow(this->window);
    SDL_Quit();
}

SDLApp *SDLApp::instance = nullptr;