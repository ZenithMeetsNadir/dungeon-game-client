#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <window/Context.hpp>

class Window {
    protected:
        Context *context;
        TTF_Font *font;

        int width;
        int height;

        bool graphicsDirty{ true };

        static const SDL_Color windowColor;

    public:
        Window(Context *context, TTF_Font *font);
        virtual ~Window();

        void invalidate();

        virtual void handleEvent(const SDL_Event &event);
        virtual void render() = 0;
};

inline void Window::invalidate() {
    graphicsDirty = true;
}

#endif