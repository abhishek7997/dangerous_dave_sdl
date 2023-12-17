#include "SDLApp.hpp"

SDLApp::SDLApp() : window(nullptr, SDL_DestroyWindow), renderer(nullptr, SDL_DestroyRenderer)
{
    std::cout << "Initializing SDL and SDL_Image\n";
    if (Initialize() < 0)
    {
        std::cout << "Could not initialize SDLApp\n";
    }
}

SDLApp &SDLApp::Get()
{
    static SDLApp instance;
    return instance;
}

int SDLApp::Initialize()
{
    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        std::cerr << "Could not initialize SDL: " << SDL_GetError() << '\n';
        return -1;
    }
    if (IMG_Init(IMG_INIT_PNG) < 0)
    {
        std::cerr << "Could not initialize SDL_Image: " << SDL_GetError() << '\n';
        return -1;
    }

    window = std::unique_ptr<SDL_Window, decltype(&SDL_DestroyWindow)>(SDL_CreateWindow(this->m_Title.c_str(), 20, 20, this->m_WindowWidth, this->m_WindowHeight, SDL_WINDOW_SHOWN), SDL_DestroyWindow);

    renderer = std::shared_ptr<SDL_Renderer>(SDL_CreateRenderer(window.get(), -1, SDL_RENDERER_ACCELERATED), SDL_DestroyRenderer);

    SDL_RenderSetScale(renderer.get(), 3, 3);
    return 0;
}

void SDLApp::SetEventCallback(std::function<void(void)> func)
{
    this->m_EventCallback = func;
}

void SDLApp::SetUpdateCallback(std::function<void(void)> func)
{
    this->m_UpdateCallback = func;
}

void SDLApp::SetRenderCallback(std::function<void(void)> func)
{
    this->m_RenderCallback = func;
}

void SDLApp::Run()
{
    Uint32 starttime, endtime, deltatime;
    while (this->isRunning)
    {
        starttime = SDL_GetTicks();

        SDL_SetRenderDrawColor(this->renderer.get(), 0x0, 0x0, 0x0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(this->renderer.get());
        this->m_EventCallback();
        this->m_UpdateCallback();
        this->m_RenderCallback();
        SDL_RenderPresent(this->renderer.get());

        endtime = SDL_GetTicks();

        deltatime = endtime - starttime;
        if (deltatime <= (1000 / this->m_MaxFrameRate))
        {
            SDL_Delay((1000 / this->m_MaxFrameRate) - deltatime);
        }
    }
}

void SDLApp::Stop()
{
    this->isRunning = false;
}

std::shared_ptr<SDL_Renderer> SDLApp::GetRenderer()
{
    return this->renderer;
}

SDLApp::~SDLApp()
{
    IMG_Quit();
    SDL_Quit();
}