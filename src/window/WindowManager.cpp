#include <window/WindowManager.hpp>
#include <window/Context.hpp>

WindowManager::WindowManager(Context *context)
    : context(context)
{ 
    service = new ServiceManager();
}

WindowManager::~WindowManager() { 
    currentWindow = nullptr;
    for (int i = 0; i < NUM_WINDOWS; ++i) {
        if (windowStorage[i]) {
            delete windowStorage[i];
            windowStorage[i] = nullptr;
        }
    }

    delete service;
}

void WindowManager::switchWindow(WindowType type) {
    int index = static_cast<int>(type);

    if (!windowStorage[index]) {
        switch (type) {
            case WindowType::lobby:
                windowStorage[index] = new LobbyWindow(context);
                break;
            case WindowType::game:
                windowStorage[index] = new GameWindow(context);
                break;
        }
    }

    if (currentWindow)
        currentWindow->leaveWindow();
        
    currentWindow = windowStorage[index];
    currentWindow->enterWindow();
}