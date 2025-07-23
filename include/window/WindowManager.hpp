#ifndef WINDOWMANAGER_HPP
#define WINDOWMANAGER_HPP

#include "Window.hpp"
#include "LobbyWindow.hpp"
#include "GameWindow.hpp"
#include "ConnectingWindow.hpp"
#include "ServiceManager.hpp"
#include "WindowRouter.hpp"
#define NUM_WINDOWS 3

struct Context;

struct WindowManager {
    enum class WindowType {
        lobby,
        connecting,
        game,
    };

    Context *context;
    ServiceManager *service;
    WindowRouter *router;

    Window *windowStorage[NUM_WINDOWS]{ nullptr, nullptr, nullptr };
    Window *currentWindow{ nullptr };
    WindowType currentWindowType;
    WindowType requestedWindowType;

    WindowManager(Context *context);
    ~WindowManager();

    void switchWindow(WindowType windowType);
    void applySwitch();
};

#endif