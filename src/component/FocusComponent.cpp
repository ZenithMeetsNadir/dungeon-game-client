#include <component/FocusComponent.hpp>

const SDL_Color FocusComponent::focusBackColor = { 0x45, 0x45, 0x45, 0xFF };
const SDL_Color FocusComponent::hoverBackColor = { 0x50, 0x50, 0x75, 0xFF };

FocusComponent::FocusComponent(Context *context)
    : Component(context) { }

FocusComponent::~FocusComponent() {
    setFocusGroup(nullptr);
}

void FocusComponent::onClick() {
    focused ? unfocus() : focus();
}

void FocusComponent::onHover() { }
void FocusComponent::onLeave() { }
void FocusComponent::onRelease() { }

void FocusComponent::setFocusGroup(FocusComponent **focusGroup) {
    if (this->focusGroup && *this->focusGroup == this)
        *this->focusGroup = nullptr;

    this->focusGroup = focusGroup;
}

void FocusComponent::focus() {
    focused = true;
    if (focusGroup) {
        if (*focusGroup)
            (*focusGroup)->unfocus(); // unfocus the previous focus

        *focusGroup = this;
    }
}

void FocusComponent::unfocus() {
    focused = false;
    if (focusGroup && *focusGroup == this)
        *focusGroup = nullptr;
}

void FocusComponent::handleMouseEvents() {
    if (!enabled)
        return;

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

    determineColor();
}

void FocusComponent::handleMouseEvents(const SDL_Event &event) {
    if (!enabled)
        return;

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

    determineColor();
}

void FocusComponent::render() {
    if (!enabled)
        return;

    if (!texture)
        createTexture();

    determineColor();

    SDL_SetRenderDrawColor(context->renderer, backColor.r, backColor.g, backColor.b, backColor.a);
    SDL_RenderFillRect(context->renderer, &bounds);

    if (texture) {
        SDL_FRect textRect = { 
            bounds.x + padding, 
            bounds.y + padding, 
            static_cast<float>(texture->w), 
            static_cast<float>(texture->h)
        };
        SDL_RenderTexture(context->renderer, texture, nullptr, &textRect);
    }
}