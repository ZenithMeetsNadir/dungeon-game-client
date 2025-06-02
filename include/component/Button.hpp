#ifndef BUTTON_HPP
#define BUTTON_HPP

#include "FocusComponent.hpp"
#include <window/Context.hpp>
#include <string>
#include <iostream>

class Button : public FocusComponent {
    protected:
        std::string text;

        void createTexture() override;
        void determineColor() override;

        static const SDL_Color pressedBackColor;

    public:
        Button(Context *context);
        virtual ~Button();

        void setText(const std::string &newText);
};

inline void Button::setText(const std::string &newText) {
    if (text != newText) {
        text = newText;
        invalidateTexture();
    }
}

#endif