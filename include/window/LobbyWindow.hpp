#ifndef LOBBYWINDOW_HPP
#define LOBBYWINDOW_HPP

#include <client/LanLobbyClient.hpp>
#include "Window.hpp"
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include <vector>
#include <string>
#include <component/Button.hpp>
#include <component/SelectButton.hpp>

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

            ServerVisual(const LanLobbyClient::GameServerInfo &serverInfo);
            ~ServerVisual();
        };

        bool mousePressed{ false };

        LanLobbyClient *lanLobby;

        std::vector<ServerVisual *> serverVisuals;
        SDL_Texture *serverList;

        SelectButton *dummyButt;
        SelectButton *playButtonComponent;
        bool *playSelectGroup{ nullptr };

        /// @brief Match the queried server list with serverVisuals.
        void matchServerVisuals();
        /// @brief Destroy all textures in serverVisuals and set them to nullptr.
        void invalidateServerVisuals();
        /// @brief Create textures for all serverVisuals that do not have a texture yet.
        void updateServerVisuals();

        SDL_Texture *createServerVisual(const LanLobbyClient::GameServerInfo &serverInfo) const;

        /// @brief Update dimensions of the server list texture.
        void updateServerListTexture();
        /// @brief Prerender the server list texture.
        void prepareServerList() const;

        SDL_FPoint getServerListOffset() const;

        void handleHover();

    public: 
        LobbyWindow(Context *context, TTF_Font *font);
        virtual ~LobbyWindow();

        void handleEvent(const SDL_Event &event) override;
        void render() override;
};

inline LobbyWindow::ServerVisual::ServerVisual(const LanLobbyClient::GameServerInfo &serverInfo)
    : serverInfo(serverInfo) 
{ }

inline LobbyWindow::ServerVisual::~ServerVisual() {
    if (texture) 
        SDL_DestroyTexture(texture);
}

inline SDL_FPoint LobbyWindow::getServerListOffset() const {
    return SDL_FPoint{ 
        static_cast<float>((width - ServerVisual::width) / 2), 
        0 
    };
} 

#endif