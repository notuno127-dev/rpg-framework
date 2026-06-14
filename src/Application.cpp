#include "Application.h"

Application::~Application()
{
    shutdown();
}

bool Application::initialize()
{
    if (!SDL_Init(SDL_INIT_VIDEO)) {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return false;
    }

    window_ = SDL_CreateWindow(
        "RPG Framework",
        1280,
        720,
        SDL_WINDOW_RESIZABLE
    );

    if (window_ == nullptr) {
        SDL_Log("SDL_CreateWindow failed: %s", SDL_GetError());
        shutdown();
        return false;
    }

    renderer_ = SDL_CreateRenderer(window_, nullptr);

    if (renderer_ == nullptr) {
        SDL_Log("SDL_CreateRenderer failed: %s", SDL_GetError());
        shutdown();
        return false;
    }

    running_ = true;

    return true;
}

void Application::run()
{
    while (running_) {
        processEvents();
        render();
    }
}

void Application::shutdown()
{
    if (renderer_ != nullptr) {
        SDL_DestroyRenderer(renderer_);
        renderer_ = nullptr;
    }

    if (window_ != nullptr) {
        SDL_DestroyWindow(window_);
        window_ = nullptr;
    }

    SDL_Quit();

    running_ = false;
}

void Application::processEvents()
{
    SDL_Event event{};

    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_EVENT_QUIT) {
            running_ = false;
        }

        if (event.type == SDL_EVENT_KEY_DOWN &&
            event.key.key == SDLK_ESCAPE) {
            running_ = false;
        }
    }
}

void Application::render()
{
    SDL_SetRenderDrawColor(renderer_, 24, 28, 36, 255);
    SDL_RenderClear(renderer_);

    SDL_RenderPresent(renderer_);
}
