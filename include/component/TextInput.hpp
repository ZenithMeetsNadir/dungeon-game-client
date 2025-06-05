#ifndef TEXTINPUT_HPP
#define TEXTINPUT_HPP

#include "FocusComponent.hpp"
#include <window/Context.hpp>
#include <string>

class TextInput : public FocusComponent {
    protected:
        std::string text;
        std::string placeholder;

        bool heightLocked{ false };

        void createTexture() override;
        void determineColor() override;

        static const SDL_Color placeholderColor;

    public:
        TextInput(Context *context);
        ~TextInput();

        void setText(const std::string &newText);
        std::string getText() const;
        void setPlaceholder(const std::string &newPlaceholder);
        std::string getPlaceholder() const;
        void lockHeight();

        void focus() override;
        void unfocus() override;
        bool handleEvents(const SDL_Event &event) override;
        void render() override;
};

inline void TextInput::setText(const std::string &newText) {
    if (text != newText) {
        text = newText;
        invalidateTexture();
    }
}

inline std::string TextInput::getText() const {
    return text;
}

inline void TextInput::setPlaceholder(const std::string &newPlaceholder) {
    if (placeholder != newPlaceholder) {
        placeholder = newPlaceholder;
        invalidateTexture();
    }
}

inline std::string TextInput::getPlaceholder() const {
    return placeholder;
}

inline void TextInput::lockHeight() {
    heightLocked = true;
}

#endif