#ifndef LOBBYWINDOW_HPP
#define LOBBYWINDOW_HPP

#include <client/LanLobbyClient.hpp>
#include <sdls.h>
#include "Window.hpp"
#include <vector>
#include <string>
#include <exception>
#include <component/Button.hpp>
#include <component/SelectButton.hpp>
#include <component/FocusComponent.hpp>
#include <component/textInput/TextInput.hpp>
#include <component/textInput/IpInput.hpp>
#include <component/PauseOverlay.hpp>
#include <util/Dotenv.hpp>
#include <exception/DotenvException.hpp>
#include <exception/NetworkInitException.hpp>

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

        LanLobbyClient *lanLobby{ nullptr };

        std::vector<ServerVisual *> serverVisuals;
        SDL_Texture *serverList{ nullptr};
        bool serverVisualsUpToDate{ false }; 
        bool serverListDirty{ true };
        bool *modeSelectGroup{ nullptr };
        FocusComponent *focusGroup{ nullptr };

        SelectButton *remoteServer{ nullptr };
        IpInput *remoteIp{ nullptr};
        TextInput *playerName{ nullptr};
        Button *playButton{ nullptr };
        Button *quitButton{ nullptr};
        
        void onRemoteIpTextChanged();
        void onPlayClick();
        void onQuitClick();

        /// @brief Match the queried server list with serverVisuals.
        void matchServerVisuals();

        /// @brief Invalidate the server list texture, so it will be recreated on the next render.
        void invalidateServerList();
        /// @brief Update dimensions of the server list texture.
        void updateServerListDimenstions();
        /// @brief Prerender the server list texture.
        void prepareServerList() const;

        SDL_FPoint getServerListOffset() const;

        void clearLobbyVolatileState();

    public: 
        LobbyWindow(Context *context);
        virtual ~LobbyWindow();

        void enterWindow() override;
        void leaveWindow() override;
        void handleEvent(const SDL_Event &event) override;
        void compute() override;
        void render() override;
};

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