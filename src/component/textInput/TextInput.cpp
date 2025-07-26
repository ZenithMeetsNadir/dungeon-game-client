#include <component/textInput/TextInput.hpp>

TextInput::TextInput(Context *context)
    : FocusComponent(context)
{ 
    foreColor = mutedColor;
}

TextInput::~TextInput() { }

void TextInput::createTexture() {
    std::string displayText = text.empty() ? placeholder : text;
    
    SDL_Surface *surface = TTF_RenderText_Solid(
        Context::font_psp2,
        displayText.c_str(),
        displayText.length(),
        foreColor
    );

    if (!surface) {
        std::cerr << "Failed to create text input surface: " << SDL_GetError() << std::endl;
        return;
    }
    
    texture = SDL_CreateTextureFromSurface(context->renderer, surface);
    if (!texture)
        std::cerr << "Failed to create texture from text input surface: " << SDL_GetError() << std::endl;
    else if (!heightLocked)
        bounds.h = texture->h + 2 * padding;

    SDL_DestroySurface(surface);
}

void TextInput::determineColor() {
    foreColor = text.empty() ? mutedColor : textColor;
    backColor = focused || hovered ? focusBackColor : idleColor;
}

bool TextInput::isValidExplicitCheck() const { 
    return true;
}

void TextInput::focus() {
    FocusComponent::focus();

    SDL_StartTextInput(context->window);
}

void TextInput::unfocus() {
    FocusComponent::unfocus();

    SDL_StopTextInput(context->window);
}

bool TextInput::isValid() const {
    if (!(isValidCache & VALIDATION_ALREADY_RUN)) {
        // too lazy to adjust and remove all the const qualifiers - it's a fucking validation function caching its output
        auto notSoConstThis = const_cast<TextInput *>(this);
        notSoConstThis->isValidCache |= VALIDATION_ALREADY_RUN;
        notSoConstThis->isValidCache |= isValidExplicitCheck() & IS_VALID_MASK;
    }
    
    return isValidCache & IS_VALID_MASK;
}

bool TextInput::handleEvents(const SDL_Event &event) {
    bool dirty = FocusComponent::handleEvents(event);
    bool textChanged = false;
    switch (event.type) {
        case SDL_EVENT_TEXT_INPUT:
            if (focused) {
                text += event.text.text;

                invalidateTexture();
                textChanged |= true;
            }
            break;
        case SDL_EVENT_KEY_DOWN:
            if (focused && event.key.key == SDLK_BACKSPACE && !text.empty()) {
                text.pop_back();

                invalidateTexture();
                textChanged |= true;
            }
            break;
    }

    if (textChanged) {
        invalidateIsValidCache();
        if (onTextChangedCallBack)
            onTextChangedCallBack();
    }

    return dirty | textChanged;
}

void TextInput::render() {
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
            bounds.y + bounds.h - texture->h - padding,
            static_cast<float>(texture->w), 
            static_cast<float>(texture->h)
        };
        SDL_RenderTexture(context->renderer, texture, nullptr, &textRect);
    }
}