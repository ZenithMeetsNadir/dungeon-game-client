#include <component/Button.hpp>

const SDL_Color Button::textColor = { 255, 255, 255, 255 };
const SDL_Color Button::idleColor = { 60, 60, 60, 255 };
const SDL_Color Button::hoverColor = { 80, 80, 120, 255 };
const SDL_Color Button::pressedColor = { 120, 120, 180, 255 };

Button::Button(SDL_Renderer *renderer, const std::string &text)
    : text(text), renderer(renderer)
{
    
}

Button::~Button() {
    if (texture)
        SDL_DestroyTexture(texture);
}

void Button::createTexture() {
    if (!texture) {
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

        texture = SDL_CreateTextureFromSurface(renderer, surface);
        if (!texture)
            std::cerr << "Failed to create texture from button text surface: " << SDL_GetError() << std::endl;
        else {
            if (bounds.w < 0)
                bounds.w = surface->w + 2 * padding;

            bounds.h = surface->h + 2 * padding;
        }

        SDL_DestroySurface(surface);
    }
}

void Button::determineBackColor() {
    backColor = pressed ? pressedColor : (hovered ? hoverColor : idleColor);
}

void Button::onHover() { }
void Button::onLeave() { }
void Button::onClick() { }
void Button::onRelease() { }

void Button::handleMouseEvents() {
    float mx, my;
    SDL_MouseButtonFlags mouseState = SDL_GetMouseState(&mx, &my);

    mx -= relPoint.x;
    my -= relPoint.y;
    SDL_FPoint mousePos = { mx, my };

    bool oldHovered = hovered;
    bool oldPressed = pressed;
    hovered = SDL_PointInRectFloat(&mousePos, &bounds);
    pressed =  hovered && mouseState & SDL_BUTTON_MASK(SDL_BUTTON_LEFT);

    if (hovered != oldHovered)
        hovered ? onHover() : onLeave();

    if (pressed != oldPressed) 
        pressed ? onClick() : onRelease();

    determineBackColor();
}

void Button::handleMouseEvents(const SDL_Event &event) {
    bool oldHovered = hovered;
    SDL_FPoint mousePos = { event.motion.x - relPoint.x, event.motion.y - relPoint.y };

    switch (event.type) {
        case SDL_EVENT_MOUSE_MOTION:
            hovered = SDL_PointInRectFloat(&mousePos, &bounds);

            if (hovered != oldHovered)
                hovered ? onHover() : onLeave();
            break;

        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            if (event.button.button == SDL_BUTTON_LEFT && hovered) {
                pressed = true;
                onClick();
            }
            break;

        case SDL_EVENT_MOUSE_BUTTON_UP:
            if (event.button.button == SDL_BUTTON_LEFT && pressed) {
                pressed = false;
                onRelease();
            }
            break;
    }

    determineBackColor();
}

void Button::render() {
    if (!texture)
        createTexture();

    SDL_SetRenderDrawColor(renderer, backColor.r, backColor.g, backColor.b, backColor.a);
    SDL_RenderFillRect(renderer, &bounds);

    if (texture) {
        SDL_FRect textRect = { 
            bounds.x + padding, 
            bounds.y + padding, 
            static_cast<float>(texture->w), 
            static_cast<float>(texture->h)
        };
        SDL_RenderTexture(renderer, texture, nullptr, &textRect);
    }
}