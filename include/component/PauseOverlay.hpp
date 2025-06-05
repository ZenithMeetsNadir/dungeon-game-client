#ifndef PAUSEOVERLAY_HPP
#define PAUSEOVERLAY_HPP

#include "Component.hpp"
#include "FocusComponent.hpp"
#include "Button.hpp"
#include <window/Context.hpp>
#include <sdls.h>
#include <functional>

class PauseOverlay : public Component {
    protected:
        Button *resume;
        Button *quit;

        void createTexture() override;

        void onResumeClick();
        void onQuitClick();

        static const int gap{ 20 };

    public:
        PauseOverlay(Context *context);
        ~PauseOverlay();

        void setControlsFocusGroup(FocusComponent **focusGroup);
        void clearState() override;
        void clearVolatileState() override;
        void attach() override;
        void detach() override;

        bool handleEvents(const SDL_Event &event) override;
        void render() override;
};

inline void PauseOverlay::setControlsFocusGroup(FocusComponent **focusGroup) {
    resume->setFocusGroup(focusGroup);
    quit->setFocusGroup(focusGroup);
}

#endif