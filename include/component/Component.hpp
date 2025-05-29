#ifndef COMPONENT_HPP
#define COMPONENT_HPP

#include <sdls.h>

class Component {
    protected:
        SDL_Renderer *renderer;
        SDL_Texture *texture{ nullptr };
        SDL_FRect bounds{ 0, 0, 0, 0 };
        SDL_FPoint relPoint{ 0, 0 };

        virtual void createTexture() = 0;
        void invalidateTexture();

        static const SDL_Color textColor;

    public:
        Component(SDL_Renderer *renderer);
        virtual ~Component();

        void setBounds(const SDL_FRect &newBounds);
        SDL_FRect getBounds() const;
        void setRelPoint(const SDL_FPoint &point);
        SDL_FPoint getRelPoint() const;
        SDL_Texture *queryTexture();

        virtual void handleMouseEvents() = 0;
        virtual void handleMouseEvents(const SDL_Event &event) = 0;
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

#endif