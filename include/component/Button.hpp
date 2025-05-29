#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <window/Context.hpp>
#include <string>
#include <iostream>

class Button {
    protected:
        std::string text;
        SDL_FRect bounds{ 0, 0, 0, 0 };
        SDL_FPoint relPoint{ 0, 0 };
        SDL_Color backColor{ idleColor };
        SDL_Texture *texture{ nullptr };
        SDL_Renderer *renderer;

        bool hovered{ false };
        bool pressed{ false };

        void createTexture();
        void invalidateTexture();

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
        static const SDL_Color textColor;
        static const SDL_Color idleColor;
        static const SDL_Color hoverColor;
        static const SDL_Color pressedColor;

    public:
        Button(SDL_Renderer *renderer, const std::string &text = "");
        virtual ~Button();

        void setText(const std::string &newText);
        void setBounds(float x = 0, float y = 0, float w = -1);
        SDL_FRect getBounds() const;
        /// @brief set w to -1 to use the width of the text.
        void setWidth(float w = -1);
        void setPos(float x, float y);
        void setRelPoint(SDL_FPoint point);
        SDL_Texture *queryTexture();

        bool isHovered() const;
        bool isPressed() const;

        virtual void handleMouseEvents();
        virtual void handleMouseEvents(const SDL_Event &event);
        virtual void render();
};

inline void Button::invalidateTexture() {
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}

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

inline void Button::setRelPoint(SDL_FPoint point) {
    relPoint = point;
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

inline SDL_FRect Button::getBounds() const {
    return bounds;
}

inline SDL_Texture *Button::queryTexture() {
    if (!texture)
        createTexture();
    
    return texture;
}

inline bool Button::isHovered() const {
    return hovered;
}

inline bool Button::isPressed() const {
    return hovered && pressed;
}

#endif