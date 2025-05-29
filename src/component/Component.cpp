#include <component/Component.hpp>

const SDL_Color Component::textColor = { 255, 255, 255, 255 };

Component::Component(SDL_Renderer *renderer)
    : renderer(renderer) 
{ }

Component::~Component() {
    if (texture)
        SDL_DestroyTexture(texture);
}