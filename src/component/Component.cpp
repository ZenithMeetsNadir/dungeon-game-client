#include <component/Component.hpp>

const SDL_Color Component::idleColor = { 0x30, 0x30, 0x30, 0xFF };
const SDL_Color Component::textColor = { 255, 255, 255, 255 };

Component::Component(Context *context)
    : context(context) 
{ }

Component::~Component() {
    if (texture)
        SDL_DestroyTexture(texture);
}

void Component::attach() {
    attached = true;
}

void Component::detach() {
    attached = false;
    clearState();
}

bool Component::handleMouseEvents() {
    return false;
}