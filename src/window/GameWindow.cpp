#include <window/GameWindow.hpp>
#include <window/WindowManager.hpp>

GameWindow::GameWindow(Context *context)
    : Window(context)
{
    width = context->width;
    height = context->height;

    pauseOverlay = new PauseOverlay(context);
    pauseOverlay->setControlsFocusGroup(&focusGroup);

    updateDimensions();
}

GameWindow::~GameWindow() { }

void GameWindow::updateDimensions() {
    SDL_FRect pauseOverlayBounds{
        0, 0,
        static_cast<float>(width),
        static_cast<float>(height)
    };
    pauseOverlay->setBounds(pauseOverlayBounds);

    invalidate();
}

void GameWindow::handleEvent(const SDL_Event &event) {
    Window::handleEvent(event);

    switch (event.type) {
        case SDL_EVENT_WINDOW_RESIZED:
            updateDimensions();
            break;
    }

    bool dirty = pauseOverlay->handleEvents(event);

    if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE) {
        pauseOverlay->isAttached() ? pauseOverlay->detach() : pauseOverlay->attach();

        if (pauseOverlay->isAttached()) {
            pauseOverlay->clearVolatileState();
        }     

        forceMotionRefresh();
        dirty |= true;
    }

    if (dirty)
        invalidate();

    if (!pauseOverlay->isAttached()) {
        
    }
}

void GameWindow::render() {
    if (!graphicsDirty)
        return;

    SDL_SetRenderDrawColor(context->renderer, windowColor.r, windowColor.g, windowColor.b, windowColor.a);
    SDL_RenderClear(context->renderer);

    pauseOverlay->render();

    SDL_RenderPresent(context->renderer);
}