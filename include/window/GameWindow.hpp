#ifndef GAMEWINDOW_HPP
#define GAMEWINDOW_HPP

#include "Context.hpp"
#include "Window.hpp"
#include <sdls.h>
#include <component/FocusComponent.hpp>
#include <component/PauseOverlay.hpp>

class GameWindow : public Window {
    protected:
        FocusComponent *focusGroup{ nullptr };

        PauseOverlay *pauseOverlay;

        void updateDimensions();

    public:
        GameWindow(Context *context);
        ~GameWindow();

        void handleEvent(const SDL_Event &event) override;
        void render() override;
};

#endif