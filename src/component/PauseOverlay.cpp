#include <component/PauseOverlay.hpp>

PauseOverlay::PauseOverlay(Context *context)
    : Component(context)
{
    attached = false;

    resume = new Button(context);
    resume->setText("Resume");
    resume->setWidth();
    resume->queryTexture();
    resume->setOnClickListener([this]() { onResumeClick(); });

    quit = new Button(context);
    quit->setText("Quit");
    quit->setWidth();
    quit->queryTexture();
    quit->setOnClickListener([this]() { onQuitClick(); });
}

PauseOverlay::~PauseOverlay() {
    delete resume;
    delete quit;
}

void PauseOverlay::clearState() {
    resume->clearState();
    quit->clearState();
}

void PauseOverlay::clearVolatileState() {
    resume->clearVolatileState();
    quit->clearVolatileState();
}

void PauseOverlay::attach() {
    Component::attach();
    resume->destroyFocus();
}

void PauseOverlay::detach() {
    Component::detach();
    resume->destroyFocus();
}

void PauseOverlay::createTexture() { }

bool PauseOverlay::handleEvents(const SDL_Event &event) {
    return attached && (resume->handleEvents(event) || quit->handleEvents(event));
}

void PauseOverlay::onResumeClick() {
    detach();
}

void PauseOverlay::onQuitClick() {
    SDL_Event quitEvent;
    quitEvent.type = SDL_EVENT_QUIT;
    SDL_PushEvent(&quitEvent);
}

void PauseOverlay::render() {
    if (!attached)
        return;

    SDL_SetRenderDrawColor(context->renderer, 0, 0, 0, 0x80);
    SDL_SetRenderDrawBlendMode(context->renderer, SDL_BLENDMODE_BLEND);
    SDL_RenderFillRect(context->renderer, &bounds);

    SDL_FRect quitBounds = quit->getBounds();
    float y = bounds.h - quitBounds.h - gap;
    quit->setPos(
        (bounds.w - quitBounds.w) / 2.f,
        y
    );

    quit->render();

    y -= quitBounds.h + gap;

    SDL_FRect resumeBounds = resume->getBounds();
    resume->setPos(
        (bounds.w - resumeBounds.w) / 2.f,
        y
    ); 

    resume->render();
}