#include <component/PauseOverlay.hpp>

PauseOverlay::PauseOverlay(SDL_Renderer *renderer)
    : Component(renderer)
{
    resume = new Button(renderer, "Resume");
    resume->setWidth();
    resume->queryTexture();

    quit = new Button(renderer, "Quit");
    quit->setWidth();
    quit->queryTexture();
}

PauseOverlay::~PauseOverlay() {
    delete resume;
    delete quit;
}