#include <window/Window.hpp>

const SDL_Color Window::windowColor = { 0x20, 0x20, 0x20, 0xFF };

Window::Window(Context *context, TTF_Font *font)
    : context(context), font(font)
{ }

Window::~Window() { }

void Window::handleEvent(const SDL_Event &event) {
    switch (event.type) {
        case SDL_EVENT_WINDOW_RESIZED:
            width = event.window.data1;
            height = event.window.data2;
            break;
    }
}