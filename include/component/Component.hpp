#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include <window/Context.hpp>
#include <sdls.h>

class Component {
    protected:
        Context *context;
        SDL_Texture *texture{ nullptr };
        SDL_FRect bounds{ 0, 0, 0, 0 };
        SDL_FPoint relPoint{ 0, 0 };
        int padding{ defaultPadding };
        bool attached{ true };

        virtual void createTexture() = 0;
        void invalidateTexture();

        static const int defaultPadding{ 20 };
        static const SDL_Color idleColor;
        static const SDL_Color textColor;

    public:

        Component(Context *context);
        virtual ~Component();

        void setBounds(const SDL_FRect &newBounds);
        SDL_FRect getBounds() const;
        /// @brief set w to -1 to use the width of the texture.
        void setBounds(float x = 0, float y = 0, float w = -1);
        /// @brief set w to -1 to use the width of the texture.
        void setWidth(float w = -1);
        void setPos(float x, float y);
        void setRelPoint(const SDL_FPoint &point);
        SDL_FPoint getRelPoint() const;
        SDL_Texture *queryTexture();

        virtual void clearState() = 0;
        virtual void clearVolatileState() = 0;
        virtual void attach();
        virtual void detach();
        bool isAttached() const;

        /// @deprecated use centralized handleEvents instead
        virtual bool handleMouseEvents();
        virtual bool handleEvents(const SDL_Event &event) = 0;
        virtual void render() = 0;
};

inline void Component::invalidateTexture() {
    if (texture) {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}

inline void Component::setBounds(const SDL_FRect &newBounds) {
    bounds = newBounds;
}

inline SDL_FRect Component::getBounds() const {
    return bounds;
}

inline void Component::setBounds(float x, float y, float w) {
    bounds.x = x;
    bounds.y = y;

    setWidth(w);
}

inline void Component::setWidth(float w) {
    if (w < 0 && texture)
        bounds.w = texture->w + 2 * padding;
    else
        bounds.w = w;
}

inline void Component::setPos(float x, float y) {
    bounds.x = x;
    bounds.y = y;
}

inline void Component::setRelPoint(const SDL_FPoint &point) {
    relPoint = point;
}

inline SDL_FPoint Component::getRelPoint() const {
    return relPoint;
}

inline SDL_Texture *Component::queryTexture() {
    if (!texture) 
        createTexture();

    return texture;
}

inline bool Component::isAttached() const {
    return attached;
}

#endif