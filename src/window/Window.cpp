#include <window/Window.hpp>
#include <window/Context.hpp>

const SDL_Color Window::windowColor = { 0x20, 0x20, 0x20, 0xFF };

Window::Window(Context *context)
    : context(context)
{ 
    width = context->initWidth;
    height = context->initHeight;
}

Window::~Window() { }

void Window::forceMotionRefresh() {
    float mx, my;
    SDL_MouseButtonFlags mouseState = SDL_GetMouseState(&mx, &my);

    SDL_Event event;
    event.type = SDL_EVENT_MOUSE_MOTION;
    event.motion.x = mx;
    event.motion.y = my;
    event.motion.xrel = 0;
    event.motion.yrel = 0;
    event.motion.windowID = SDL_GetWindowID(context->window);
    event.motion.which = SDL_TOUCH_MOUSEID;
    event.motion.timestamp = SDL_GetTicksNS();
    event.motion.state = mouseState;
    event.motion.reserved = 0;

    SDL_PeepEvents(&event, 1, SDL_ADDEVENT, SDL_EVENT_MOUSE_MOTION, SDL_EVENT_MOUSE_MOTION);
}

void Window::handleEvent(const SDL_Event &event) {
    switch (event.type) {
        case SDL_EVENT_WINDOW_RESIZED:
            width = event.window.data1;
            height = event.window.data2;
            break;
    }
}