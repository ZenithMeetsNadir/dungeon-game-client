#include <window/Context.hpp>
#include <window/WindowManager.hpp>

TTF_Font *Context::font_psp2 = nullptr;

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
        TTF_Quit();
        SDL_Quit();
        return false;
    }

    font_psp2 = TTF_OpenFont("assets/font/PressStart2P-Regular.ttf", 18);
    if (!font_psp2) {
        std::cerr << "TTF_OpenFont failed to open PressStart2P-Regular.ttf: " << SDL_GetError() << std::endl;
        TTF_Quit();
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
        SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE
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

    windowManager = new WindowManager(this);

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

    if (windowManager) {
        delete windowManager;
        windowManager = nullptr;
    }
}