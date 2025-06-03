#ifndef FOCUSCOMPONENT_HPP
#define FOCUSCOMPONENT_HPP

#include "Component.hpp"

class FocusComponent : public Component {
    protected:
        bool focused{ false };
        FocusComponent **focusGroup{ nullptr };

        bool hovered{ false };
        bool pressed{ false };

        SDL_Color foreColor{ textColor };
        SDL_Color backColor{ idleColor };

        virtual void determineColor() = 0;
        virtual void onHover();
        virtual void onLeave();
        virtual void onClick();
        virtual void onRelease();

        static const SDL_Color focusBackColor;
        static const SDL_Color hoverBackColor;

    public:
        bool enabled{ true };

        FocusComponent(Context *context);
        ~FocusComponent();

        void setFocusGroup(FocusComponent **focusGroup);
        virtual void focus();
        virtual void unfocus();
        bool isFocused() const;

        bool isHovered() const;
        bool isPressed() const;

        bool handleMouseEvents() override;
        bool handleMouseEvents(const SDL_Event &event) override;
        void render() override;
};

inline bool FocusComponent::isFocused() const {
    return focused;
}

inline bool FocusComponent::isHovered() const {
    return hovered;
}

inline bool FocusComponent::isPressed() const {
    return pressed;
}

#endif