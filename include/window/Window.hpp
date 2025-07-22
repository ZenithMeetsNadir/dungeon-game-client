#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <sdls.h>

class Context;

class Window {
    protected:
        Context *context;

        int width;
        int height;

        bool graphicsDirty{ true };

        static const SDL_Color windowColor;
        static const SDL_Color textColor;

    public:
        Window(Context *context);
        virtual ~Window();

        void invalidate();
        void forceMotionRefresh();

        virtual void enterWindow();
        virtual void leaveWindow();
        virtual void handleEvent(const SDL_Event &event);
        virtual void compute() = 0;
        virtual void render() = 0;
};

inline void Window::invalidate() {
    graphicsDirty = true;
}

#endif