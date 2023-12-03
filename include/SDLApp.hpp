#pragma once

#include <iostream>
#include <functional>
#include <SDL.h>
#include <SDL_image.h>

class SDLApp
{
public:
    static SDLApp *getInstance();
    int Initialize();
    void SetEventCallback(std::function<void(void)> func);
    void SetUpdateCallback(std::function<void(void)> func);
    void SetRenderCallback(std::function<void(void)> func);
    void Run();
    void Stop();
    SDL_Renderer *GetRenderer();
    ~SDLApp();

private:
    static SDLApp *instance;
    SDLApp();
    SDLApp(const SDLApp &) = delete;
    SDLApp &operator=(const SDLApp &) = delete;

    bool isRunning = true;
    const int m_RenderScale = 3;
    const int m_WindowWidth = 960;
    const int m_WindowHeight = 480 + 4 * 16 * m_RenderScale;
    const int m_MaxFrameRate = 30;
    SDL_Window *window = nullptr;
    SDL_Renderer *renderer = nullptr;
    std::function<void(void)> m_EventCallback = []() {};
    std::function<void(void)> m_UpdateCallback = []() {};
    std::function<void(void)> m_RenderCallback = []() {};
};