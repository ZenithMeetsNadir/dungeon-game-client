#include <component/FocusComponent.hpp>

const SDL_Color FocusComponent::focusBackColor = { 0x45, 0x45, 0x45, 0xff };
const SDL_Color FocusComponent::hoverBackColor = { 0x50, 0x50, 0x75, 0xff };
const SDL_Color FocusComponent::invalidBackColor = { 0x75, 0x40, 0x40, 0xff };

FocusComponent::FocusComponent(Context *context)
    : Component(context), onClickCallback(nullptr)
{ }

FocusComponent::~FocusComponent() {
    setFocusGroup(nullptr);
}

void FocusComponent::onHover() { }
void FocusComponent::onLeave() { }

void FocusComponent::onClick() {
    focused ? unfocus() : focus();
}

void FocusComponent::onRelease() {
    if (onClickCallback) 
        onClickCallback();
}

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

void FocusComponent::destroyFocus() {
    if (focusGroup && *focusGroup) {
        (*focusGroup)->unfocus();
        *focusGroup = nullptr;
    }
}

bool FocusComponent::enable() {
    bool enabledTemp = enabled;
    enabled = true;
    return !enabledTemp;
}

bool FocusComponent::disable() {
    bool enabledTemp = enabled;
    enabled = false;

    if (enabledTemp)
        clearVolatileState();

    return enabledTemp;
}

SDL_Texture *FocusComponent::queryTexture() {
    determineColor();
    return Component::queryTexture();
}


void FocusComponent::clearState() {
    clearVolatileState();
}

void FocusComponent::clearVolatileState() {
    unfocus();
    hovered = false;
    pressed = false;
}

bool FocusComponent::handleMouseEvents() {
    if (!attached || !enabled)
        return false;

    bool dirty = false;

    float mx, my;
    SDL_MouseButtonFlags mouseState = SDL_GetMouseState(&mx, &my);

    mx -= relPoint.x;
    my -= relPoint.y;
    SDL_FPoint mousePos = { mx, my };

    bool oldHovered = hovered;
    bool oldPressed = pressed;
    hovered = SDL_PointInRectFloat(&mousePos, &bounds);
    pressed =  hovered && mouseState & SDL_BUTTON_MASK(SDL_BUTTON_LEFT);

    if (hovered != oldHovered) {
        hovered ? onHover() : onLeave();
        dirty = true;
    }

    if (pressed != oldPressed) {
        pressed ? onClick() : onRelease();
        dirty = true;
    }

    return dirty;
}

bool FocusComponent::handleEvents(const SDL_Event &event) {
    if (!attached || !enabled)
        return false;

    bool dirty = false;

    bool oldHovered = hovered;
    SDL_FPoint mousePos = { event.motion.x - relPoint.x, event.motion.y - relPoint.y };

    switch (event.type) {
        case SDL_EVENT_MOUSE_MOTION:
            hovered = SDL_PointInRectFloat(&mousePos, &bounds);

            if (hovered != oldHovered) {
                hovered ? onHover() : onLeave();
                dirty |= true;
            }

            break;

        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            if (event.button.button == SDL_BUTTON_LEFT && hovered) {
                pressed = true;
                onClick();
                dirty |= true;
            }
            break;

        case SDL_EVENT_MOUSE_BUTTON_UP:
            if (event.button.button == SDL_BUTTON_LEFT && isPressed()) {
                pressed = false;
                onRelease();
                dirty |= true;
            }
            break;
    }

    return dirty;
}

void FocusComponent::render() {
    if (!attached)
        return;

    determineColor();

    if (!texture)
        createTexture();

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