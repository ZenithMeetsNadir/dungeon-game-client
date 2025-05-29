#include <client/LanLobbyClient.hpp>
#include <window/Context.hpp>
#include <window/LobbyWindow.hpp>
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

    LobbyWindow *lobbyWindow = new LobbyWindow(context, Context::font_psp2);

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT)
                running = false;

            lobbyWindow->handleEvent(event);
        }
        
        lobbyWindow->render();
    }

    delete lobbyWindow;

    context->destroyWindow();
    delete context;

    Context::quit();

    return 0;
}