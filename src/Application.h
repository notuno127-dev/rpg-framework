#pragma once

#include <SDL3/SDL.h>

class Application
{
public:
    Application() = default;
    ~Application();

    // コピー禁止
    Application(const Application &) = delete;
    Application &operator=(const Application &) = delete;

    bool initialize();
    void run();
    void shutdown();

private:
    void processEvents();
    void render();

    SDL_Window *window_ = nullptr;
    SDL_Renderer *renderer_ = nullptr;

    bool running_ = false;
};
