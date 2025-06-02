#include <component/Button.hpp>

const SDL_Color Button::pressedBackColor = { 120, 120, 180, 255 };

Button::Button(Context *context)
    : FocusComponent(context)
{ }

Button::~Button() {
    if (texture)
        SDL_DestroyTexture(texture);
}

void Button::createTexture() {
    SDL_Surface *surface = TTF_RenderText_Solid(
        Context::font_psp2,
        text.c_str(),
        text.length(),
        textColor
    );
    
    if (!surface) {
        std::cerr << "Failed to create button text surface: " << SDL_GetError() << std::endl;
        return;
    }

    texture = SDL_CreateTextureFromSurface(context->renderer, surface);
    if (!texture)
        std::cerr << "Failed to create texture from button text surface: " << SDL_GetError() << std::endl;
    else {
        if (bounds.w < 0)
            bounds.w = texture->w + 2 * padding;

        bounds.h = texture->h + 2 * padding;
    }

    SDL_DestroySurface(surface);
}

void Button::determineColor() {
    backColor = isPressed() ? pressedBackColor : (hovered ? hoverBackColor : idleColor);
}