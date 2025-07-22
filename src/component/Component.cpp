#include <component/Component.hpp>

const SDL_Color Component::idleColor = { 0x30, 0x30, 0x30, 0xFF };
const SDL_Color Component::textColor = { 0xff, 0xff, 0xff, 0xff };
const SDL_Color Component::mutedColor = { 0x70, 0x70, 0x70, 0xFF };

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

SDL_Texture *Component::queryTexture() {
    if (!texture) 
        createTexture();

    return texture;
}

bool Component::handleMouseEvents() {
    return false;
}