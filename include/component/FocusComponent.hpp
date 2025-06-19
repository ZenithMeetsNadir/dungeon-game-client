#ifndef FOCUSCOMPONENT_HPP
#define FOCUSCOMPONENT_HPP

#include "Component.hpp"
#include <functional>

class FocusComponent : public Component {
    protected:
        bool focused{ false };
        FocusComponent **focusGroup{ nullptr };

        bool hovered{ false };
        bool pressed{ false };

        bool enabled{ true };

        SDL_Color foreColor{ textColor };
        SDL_Color backColor{ idleColor };

        std::function<void()> onClickCallback;

        virtual void determineColor() = 0;
        virtual void onHover();
        virtual void onLeave();
        virtual void onClick();
        virtual void onRelease();

        static const SDL_Color focusBackColor;
        static const SDL_Color hoverBackColor;

    public:
        FocusComponent(Context *context);
        ~FocusComponent();

        void setFocusGroup(FocusComponent **focusGroup);
        virtual void focus();
        virtual void unfocus();
        void destroyFocus();

        bool isFocused() const;
        void setOnClickListener(const std::function<void()> &callback);

        bool isHovered() const;
        bool isPressed() const;
        bool isEnabled() const;

        virtual bool enable();
        virtual bool disable();

        void clearState() override;
        void clearVolatileState() override;

        bool handleMouseEvents() override;
        bool handleEvents(const SDL_Event &event) override;
        void render() override;
};

inline bool FocusComponent::isFocused() const {
    return focused;
}

inline void FocusComponent::setOnClickListener(const std::function<void()> &callback) {
    onClickCallback = callback;
}

inline bool FocusComponent::isHovered() const {
    return hovered;
}

inline bool FocusComponent::isPressed() const {
    return pressed && hovered;
}

inline bool FocusComponent::isEnabled() const {
    return enabled;
}

#endif