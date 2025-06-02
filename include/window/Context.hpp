#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include <GL/gl.h>
#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <SDL3_image/SDL_image.h>
#include <iostream>

struct Context {
    SDL_Window *window{ nullptr };
    SDL_Renderer *renderer{ nullptr };

    const char *title;
    int initWidth;
    int initHeight;

    static TTF_Font *font_psp2;

    Context(const char *title, int width, int height);
    ~Context();

    static bool init();
    static void quit();

    bool createWindow();
    void destroyWindow();
};

#endif