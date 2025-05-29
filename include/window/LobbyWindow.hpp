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
            SelectButton *button;
            LanLobbyClient::GameServerInfo serverInfo;

            static const int width = 600;
            static const int padding = 20;
            static const int gap = 20;
            static const SDL_Color textColor;

            ServerVisual(LobbyWindow *self, const LanLobbyClient::GameServerInfo &serverInfo);
            ~ServerVisual();
        };

        LanLobbyClient *lanLobby;

        std::vector<ServerVisual *> serverVisuals;
        SDL_Texture *serverList;
        bool serverListDirty{ true };
        bool *modeSelectGroup{ nullptr };

        SelectButton *singlePlayer;
        Button *playButton;

        /// @brief Match the queried server list with serverVisuals.
        void matchServerVisuals();

        /// @brief Invalidate the server list texture, so it will be recreated on the next render.
        void invalidateServerList();
        /// @brief Update dimensions of the server list texture.
        void updateServerListDimenstions();
        /// @brief Prerender the server list texture.
        void prepareServerList() const;

        SDL_FPoint getServerListOffset() const;

    public: 
        LobbyWindow(Context *context, TTF_Font *font);
        virtual ~LobbyWindow();

        void handleEvent(const SDL_Event &event) override;
        void render() override;
};

inline LobbyWindow::ServerVisual::ServerVisual(LobbyWindow *self, const LanLobbyClient::GameServerInfo &serverInfo)
    : serverInfo(serverInfo) 
{
    button = new SelectButton(self->context->renderer, serverInfo.name + " - " + static_cast<std::string>(serverInfo.addr));
    button->setSelectGroup(&self->modeSelectGroup);
}

inline LobbyWindow::ServerVisual::~ServerVisual() {
    delete button;
}

inline void LobbyWindow::invalidateServerList() {
    serverListDirty = true;
    invalidate();
}

inline SDL_FPoint LobbyWindow::getServerListOffset() const {
    return SDL_FPoint{ 
        static_cast<float>((width - ServerVisual::width) / 2), 
        0 
    };
} 

#endif