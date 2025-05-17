#ifndef LOBBYWINDOW_HPP
#define LOBBYWINDOW_HPP

#include <window/LanLobbyClient.hpp>
#include <window/Window.hpp>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <vector>
#include <string>

class LobbyWindow : public Window {
    protected:
        struct ServerVisual {
            SDL_Texture *texture;
            LanLobbyClient::GameServerInfo serverInfo;

            static const int width = 600;
            static const int padding = 20;
            static const int gap = 20;
            static const SDL_Color textColor;

            ServerVisual(const LobbyWindow *lobbyWindow, const LanLobbyClient::GameServerInfo serverInfo) : serverInfo(serverInfo) {
                texture = lobbyWindow->createServerVisual(serverInfo);
            }
            ~ServerVisual() {
                SDL_DestroyTexture(this->texture);
            }
        };

        LanLobbyClient *lanLobby;

        std::vector<ServerVisual *> serverVisuals;
        SDL_Texture *serverList;

        void matchServerVisuals();

        SDL_Texture *createServerVisual(const LanLobbyClient::GameServerInfo &serverInfo) const;
        void createServerList() const;

    public: 
        LobbyWindow(Context *context, TTF_Font *font);
        virtual ~LobbyWindow();

        void render() override;
};

#endif