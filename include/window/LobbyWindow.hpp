#ifndef LOBBYWINDOW_HPP
#define LOBBYWINDOW_HPP

#include <client/LanLobbyClient.hpp>
#include "Window.hpp"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <vector>
#include <string>

class LobbyWindow : public Window {
    protected:
        struct ServerVisual {
            SDL_FRect box;
            SDL_Texture *texture{ nullptr };
            LanLobbyClient::GameServerInfo serverInfo;

            bool hovered{ false };

            static const int width = 600;
            static const int padding = 20;
            static const int gap = 20;
            static const SDL_Color textColor;

            ServerVisual(const LobbyWindow *lobbyWindow, const LanLobbyClient::GameServerInfo serverInfo) : serverInfo(serverInfo) { }
            ~ServerVisual() {
                if (texture) 
                    SDL_DestroyTexture(texture);
            }
        };

        bool mousePressed{ false };

        LanLobbyClient *lanLobby;

        std::vector<ServerVisual *> serverVisuals;
        SDL_Texture *serverList;

        void matchServerVisuals();
        void invalidateServerVisuals();
        void updateServerVisuals();

        SDL_Texture *createServerVisual(const LanLobbyClient::GameServerInfo &serverInfo) const;

        void updateServerListTexture();
        void prepareServerList() const;

        SDL_FPoint getServerListOffset() const {
            return SDL_FPoint{ 
                static_cast<float>((width - ServerVisual::width) / 2), 
                0 
            };
        }

        void handleHover();

    public: 
        LobbyWindow(Context *context, TTF_Font *font);
        virtual ~LobbyWindow();

        void handleEvent(const SDL_Event &event) override;
        void render() override;
};

#endif