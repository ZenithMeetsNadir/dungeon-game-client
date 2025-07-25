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

        PauseOverlay *pauseOverlay{ nullptr };

        void updateDimensions();

    public:
        GameWindow(Context *context);
        ~GameWindow();

        void enterWindow() override;
        void leaveWindow() override;
        void handleEvent(const SDL_Event &event) override;
        void compute() override;
        void render() override;
};

#endif