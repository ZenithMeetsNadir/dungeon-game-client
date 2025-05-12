#include "udp/udpServer.hpp"
#include <GL/gl.h>
#include <SDL3/SDL.h>
#include <iostream>

void dispatch(const UdpServer *const server, IPv4Addr addr, const char *data, size_t size) {
    std::cout << "Received data from " << inet_ntoa(addr.addr) << ":" << ntohs(addr.port) << " - " << std::string(data, size) << std::endl;
}

int main(int argc, char **argv) {
    UdpServer server(IPv4Addr("0.0.0.0", 6969));
    if (server.open() == SOCKET_ERROR) {
        std::cerr << "failed to open UDP server" << std::endl;
        return 1;
    }

    server.enableBroadcast();
    server.setDispatchFunc(dispatch);

    server.listen();

    std::cout << "press ENTER to stop the server..." << std::endl;
    std::cin.get();

    server.close();

    return 0;

    /*if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

    SDL_Window *window = SDL_CreateWindow(
        "Dungeon Game",
        800, 600,
        SDL_WINDOW_OPENGL
    );

    if (!window) {
        std::cerr << "SDL_CreateWindow failed: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(
        window,
        nullptr
    );

    if (!renderer) {
        std::cerr << "SDL_CreateRenderer failed: " << SDL_GetError() << std::endl;
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    bool running = true;
    SDL_Event event;

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT)
                running = false;
        }

        SDL_SetRenderDrawColor(renderer, 30, 30, 30, SDL_ALPHA_OPAQUE); 
        SDL_RenderClear(renderer);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;*/
}