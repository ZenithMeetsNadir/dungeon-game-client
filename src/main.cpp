#include <window/Context.hpp>
#include <window/LobbyWindow.hpp>
#include <window/WindowManager.hpp>
#include <exception/NetworkInitException.hpp>
#include <iostream>

int main(int argc, char **argv) {
    if (!Context::init()) {
        std::cerr << "Failed to initialize SDL" << std::endl;
        return 1;
    }

    Context *context = new Context("Dungeon game", 800, 600);
    if (!context->createWindow()) {
        std::cerr << "Failed to create window" << std::endl;
        return 1;
    }

    context->windowManager->switchWindow(WindowManager::WindowType::lobby);

    bool running = true;
    SDL_Event event;

    try {
        while (running) {
            context->windowManager->applySwitch();
            context->windowManager->currentWindow->satisfyMotionRefreshRequest();

            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_EVENT_QUIT)
                    running = false;

                context->windowManager->currentWindow->handleEvent(event);
            }

            context->windowManager->currentWindow->compute();
            context->windowManager->currentWindow->render();
        }
    }
    catch (const NetworkInitException &e) {
        std::cerr << "NetworkInitException: shutting down" << std::endl;
        running = false;
    }

    context->destroyWindow();
    delete context;

    Context::quit();

    std::cout << "shut down gracefully" << std::endl;

    return 0;
}