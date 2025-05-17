#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <window/Context.hpp>

class Window {
    protected:
        Context *context;
        TTF_Font *font;

    public:
        Window(Context *context, TTF_Font *font);
        virtual ~Window();

        virtual void render() = 0;
};

#endif