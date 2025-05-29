#ifndef PAUSEOVERLAY_HPP
#define PAUSEOVERLAY_HPP

#include "Component.hpp"
#include "Button.hpp"
#include <sdls.h>

class PauseOverlay : public Component {
    protected:
        Button *resume;
        Button *quit;

    public:
        PauseOverlay(SDL_Renderer *renderer);
        ~PauseOverlay();

        void handleMouseEvents(const SDL_Event &event) override;
        void render() override;
};

#endif