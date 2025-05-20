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

    public:
        Window(Context *context, TTF_Font *font);
        virtual ~Window();

        virtual void handleEvent(const SDL_Event &event);
        virtual void render() = 0;
};

#endif