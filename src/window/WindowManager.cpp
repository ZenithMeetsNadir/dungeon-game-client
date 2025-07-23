#include <window/WindowManager.hpp>
#include <window/Context.hpp>

WindowManager::WindowManager(Context *context)
    : context(context)
{ 
    service = new ServiceManager();
    router = new WindowRouter(context);
}

WindowManager::~WindowManager() { 
    currentWindow = nullptr;
    for (int i = 0; i < NUM_WINDOWS; ++i) {
        if (windowStorage[i]) {
            delete windowStorage[i];
            windowStorage[i] = nullptr;
        }
    }

    delete router;
    delete service;
}

void WindowManager::switchWindow(WindowType windowType) {
    requestedWindowType = windowType;
}

void WindowManager::applySwitch() {
    if (requestedWindowType != currentWindowType) {
        int index = static_cast<int>(requestedWindowType);

        if (!windowStorage[index]) {
            switch (requestedWindowType) {
                case WindowType::lobby:
                    windowStorage[index] = new LobbyWindow(context);
                    break;
                case WindowType::connecting:
                    windowStorage[index] = new ConnectingWindow(context);
                    break;
                case WindowType::game:
                    windowStorage[index] = new GameWindow(context);
                    break;
            }
        }

        if (currentWindow)
            currentWindow->leaveWindow();
            
        currentWindow = windowStorage[index];
        currentWindowType = requestedWindowType;
        currentWindow->enterWindow();
    }
}