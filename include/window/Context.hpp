#ifndef CONTEXT_HPP
#define CONTEXT_HPP

#include <service/ServiceManager.hpp>
#include <sdls.h>
#include <iostream>

struct WindowManager;

struct Context {
    SDL_Window *window{ nullptr };
    SDL_Renderer *renderer{ nullptr };

    /// created by `createWindow`, makes no sense to do that in the constructor without the underlying SDL_Window being created
    /// call `destroyWindow` to release
    WindowManager *windowManager{ nullptr };
    ServiceManager *service;

    const char *title;
    int width;
    int height;

    static TTF_Font *font_psp2;

    Context(const char *title, int width, int height);
    ~Context();

    static bool init();
    static void quit();

    bool createWindow();
    void destroyWindow();
};

#endif