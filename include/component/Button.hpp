#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <sdls.h>
#include <component/Component.hpp>
#include <window/Context.hpp>
#include <string>
#include <iostream>

class Button : public Component {
    protected:
        std::string text;
        SDL_Color backColor{ idleColor };

        bool hovered{ false };
        bool pressed{ false };

        void createTexture() override;

        virtual void determineBackColor();
        /// @brief Empty in base class, no need to call the base implementation when overriding.
        virtual void onHover();
        /// @brief Empty in base class, no need to call the base implementation when overriding.
        virtual void onLeave();
        /// @brief Empty in base class, no need to call the base implementation when overriding.
        virtual void onClick();
        /// @brief Empty in base class, no need to call the base implementation when overriding.
        virtual void onRelease();

        static const int padding = 20;
        static const SDL_Color idleColor;
        static const SDL_Color hoverColor;
        static const SDL_Color pressedColor;

    public:
        Button(SDL_Renderer *renderer, const std::string &text = "");
        virtual ~Button();

        void setText(const std::string &newText);
        void setBounds(float x = 0, float y = 0, float w = -1);
        /// @brief set w to -1 to use the width of the text.
        void setWidth(float w = -1);
        void setPos(float x, float y);

        bool isHovered() const;
        bool isPressed() const;

        void handleMouseEvents() override;
        void handleMouseEvents(const SDL_Event &event) override;
        void render() override;
};

inline void Button::setText(const std::string &newText) {
    if (text != newText) {
        text = newText;
        invalidateTexture();
    }
}

inline void Button::setBounds(float x, float y, float w) {
    bounds.x = x;
    bounds.y = y;

    setWidth(w);
}

inline void Button::setWidth(float w) {
    if (w < 0 && texture)
        bounds.w = texture->w + 2 * padding;
    else
        bounds.w = w;
}

inline void Button::setPos(float x, float y) {
    bounds.x = x;
    bounds.y = y;
}

inline bool Button::isHovered() const {
    return hovered;
}

inline bool Button::isPressed() const {
    return hovered && pressed;
}

#endif