#include <window/LanLobbyClient.hpp>
#include <window/Context.hpp>
#include <iostream>

//Context context("Dungeon game", 800, 600);

int main(int argc, char **argv) {
    LanLobbyClient lobby;
    if (!lobby.open()) {
        std::cerr << "Failed to open UDP server" << std::endl;
        return 1;
    }

    std::cout << "press ENTER to stop the server..." << std::endl;
    std::cin.get();

    lobby.close();

    return 0;

    /*TTF_Font *font = TTF_OpenFont("C:\\Users\\marti\\programy\\c_cpp\\dungeon_game_client\\assets\\font\\constan.ttf", 64);
    if (!font) {
        std::cerr << "TTF_OpenFont failed: " << SDL_GetError() << std::endl;
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }

    SDL_Color white{ 255, 255, 255, 255 };

    std::string text = "BOING";
    SDL_Surface *surface = TTF_RenderText_Solid(font, text.c_str(), text.length(), white);
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_DestroySurface(surface);*/

    /*Context::init();
    context.createWindow();

    bool running = true;
    SDL_Event event;
    //SDL_FRect rect{ 0, 0, static_cast<float>(surface->w), static_cast<float>(surface->h) };

    while (running) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_EVENT_QUIT)
                running = false;
        }

        SDL_SetRenderDrawColor(context.renderer, 30, 30, 30, SDL_ALPHA_OPAQUE); 
        SDL_RenderClear(context.renderer);

        SDL_RenderPresent(context.renderer);
    }

    context.destroyWindow();
    Context::quit();*/

    //return 0;
}