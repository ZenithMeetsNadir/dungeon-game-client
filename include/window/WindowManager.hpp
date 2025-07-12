#ifndef WINDOWMANAGER_HPP
#define WINDOWMANAGER_HPP

#include "Window.hpp"
#include "LobbyWindow.hpp"
#include "GameWindow.hpp"
#include "ServiceManager.hpp"
#define NUM_WINDOWS 2

struct Context;

struct WindowManager {
    enum class WindowType {
        lobby,
        game,
    };

    Context *context;
    ServiceManager *service;

    Window *windowStorage[NUM_WINDOWS]{ nullptr, nullptr };
    Window *currentWindow{ nullptr };

    WindowManager(Context *context);
    ~WindowManager();

    void switchWindow(WindowType type);
};

#endif