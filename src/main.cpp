#include "udp/udpServer.hpp"
#include "udp/DataPacker.hpp"
#include <GL/gl.h>
#include <SDL3/SDL.h>
#include <iostream>

u_short port = 6969;
DataPacker dp("nejtajnejsiheslouwu", "uwu");
UdpServer udpSearch(IPv4Addr("0.0.0.0", 0));

size_t tickCtr = 0;
std::atomic_bool searchRunning = true;

void dispatchSearchResponse(const UdpServer *const server, IPv4Addr addr, const char *data, size_t size);
void broadcastSearchLoop();

int main(int argc, char **argv) {
    dp.keyValueMode();

    if (udpSearch.open() == SOCKET_ERROR) {
        std::cerr << "failed to open UDP server" << std::endl;
        return 1;
    }

    udpSearch.enableBroadcast();
    udpSearch.setDispatchFunc(dispatchSearchResponse);

    udpSearch.listen();

    std::thread broadcastThread(broadcastSearchLoop);

    std::cout << "press ENTER to stop the server..." << std::endl;
    std::cin.get();

    searchRunning.store(false, std::memory_order_release);
    broadcastThread.join();

    udpSearch.close();

    return 0;

    /*if (!SDL_Init(SDL_INIT_VIDEO)) {
        std::cerr << "SDL_Init failed: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return 1;
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);

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

void dispatchSearchResponse(const UdpServer *const server, IPv4Addr addr, const char *data, size_t size) {  
    std::cout << "received data from " << inet_ntoa(addr.addr) << ":" << ntohs(addr.port) << std::endl;
    
    std::string decoded = dp.whichevercrypt(std::string(data, size));
    std::cout << "decoded data: " << decoded << std::endl;

    if (!dp.verify(decoded)) {
        std::cerr << "invalid data received" << std::endl;
        return;
    }

    std::string serverName = dp.valueOf(decoded, "sname");
    if (!serverName.empty()) {
        size_t tick = std::stoll(dp.valueOf(decoded, "t"));

        std::cout << serverName << ": " << inet_ntoa(addr.addr) << std::endl;
    }
}

void broadcastSearchLoop() {
    IPv4Addr brdcast("255.255.255.255", 6969);

    std::cout << "broadcasting search trigger..." << std::endl;
    while (searchRunning.load(std::memory_order_acquire)) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));

        std::string message = dp.message();
        dp.msgAppend(message, "s", "");
        dp.msgAppend(message, "t", std::to_string(tickCtr++));

        std::string questionExistence = dp.whichevercrypt(message);

        udpSearch.sendTo(brdcast, questionExistence.c_str(), questionExistence.length());
    }
}