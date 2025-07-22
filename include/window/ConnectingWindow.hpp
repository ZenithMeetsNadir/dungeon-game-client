#ifndef CONNECTINGWINDOW_HPP
#define CONNECTINGWINDOW_HPP

#include "Window.hpp"
#include <component/FocusComponent.hpp>
#include <component/Button.hpp>
#include <atomic>
#include <thread>
#include <exception/NetworkInitException.hpp>

#define CONNECTING_WINDOW_STATE_PENDING 0
#define CONNECTING_WINDOW_STATE_SUCCESS 1
#define CONNECTING_WINDOW_STATE_TIMEOUT 2

class ConnectingWindow : public Window {
    protected: 
        /// @brief state of the connection attempt, 0 = pending, 1 = success, 2 = timeout
        std::atomic_char state{ CONNECTING_WINDOW_STATE_PENDING };

        SDL_Texture *connectingTexture{ nullptr };

        FocusComponent *focusGroup{ nullptr };
        Button *abortButton{ nullptr };
        Button *quitButton{ nullptr };

        void onAbortClick();
        void onQuitClick();

        void createConnectingTexture();

    public: 
        ConnectingWindow(Context *context);
        ~ConnectingWindow();

        void enterWindow() override;
        void leaveWindow() override;
        void handleEvent(const SDL_Event &event) override;
        void compute() override;
        void render() override;
};

#endif