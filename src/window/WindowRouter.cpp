#include <window/WindowRouter.hpp>
#include <window/WindowManager.hpp>

WindowRouter::WindowRouter(Context *context) 
    : context(context) 
{ }

WindowRouter::~WindowRouter() { }

void WindowRouter::flowDirectionViolation(const char *action) {
    std::cerr << "flow direction violation: " << action << std::endl;
}

void WindowRouter::gameConnect() {
    if (context->windowManager->currentWindowType == WindowManager::WindowType::lobby) {
        context->windowManager->switchWindow(WindowManager::WindowType::connecting);
        return;
    }

    flowDirectionViolation("gameConnect");
}

void WindowRouter::gameDisconnect() {
    if (context->windowManager->currentWindowType == WindowManager::WindowType::game
    || context->windowManager->currentWindowType == WindowManager::WindowType::connecting) {
        context->windowManager->switchWindow(WindowManager::WindowType::lobby);
        return;
    }

    flowDirectionViolation("gameDisconnect");
}

void WindowRouter::gameLoad() {
    if (context->windowManager->currentWindowType == WindowManager::WindowType::connecting) {
        context->windowManager->switchWindow(WindowManager::WindowType::game);
        return;
    }

    flowDirectionViolation("gameLoad");
}

void WindowRouter::gameExit() {
    SDL_Event event;
    event.type = SDL_EVENT_QUIT;
    SDL_PushEvent(&event); 
}