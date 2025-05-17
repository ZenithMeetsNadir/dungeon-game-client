#include <window/Window.hpp>

Window::Window(Context *context, TTF_Font *font)
    : context(context), font(font) 
{ }

Window::~Window() { }