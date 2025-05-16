#include <window/Context.hpp>

Context::Context(const char *title, int width, int height)
    : title(title), width(width), height(height) { }

Context::~Context() { }

bool Context::init() {
    if (!SDL_Init(SDL_INIT_VIDEO)) {
            std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
            SDL_Quit();
            return false;
        }

        if (!TTF_Init()) {
            std::cerr << "TTF_Init failed" << SDL_GetError() << std::endl;
            SDL_Quit();
            return false;
        }

        return true;
}

void Context::quit() {
    TTF_Quit();
    SDL_Quit();
}

bool Context::createWindow() {
    window = SDL_CreateWindow(
        title,
        width, height,
        SDL_WINDOW_OPENGL
    );

    if (!window) {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, nullptr);

    if (!renderer) {
        std::cerr << "SDL_CreateRenderer failed: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        return false;
    }

    return true;
}

void Context::destroyWindow() {
    if (renderer) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }

    if (window) {
        SDL_DestroyWindow(window);
        window = nullptr;
    }
}