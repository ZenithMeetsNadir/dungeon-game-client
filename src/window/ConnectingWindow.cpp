#include <window/ConnectingWindow.hpp>
#include <window/WindowManager.hpp>

ConnectingWindow::ConnectingWindow(Context *context)
    : Window(context) 
{ }

ConnectingWindow::~ConnectingWindow() { 
    leaveWindow();
}

void ConnectingWindow::onAbortClick() {
    context->windowManager->service->gameClient->disconnectBlocking();
    context->windowManager->router->gameDisconnect();
}

void ConnectingWindow::onQuitClick() {
    context->windowManager->router->gameExit();
}

void ConnectingWindow::createConnectingTexture() {
    if (connectingTexture)
        SDL_DestroyTexture(connectingTexture);

    const char *connectingText = "Connecting...";
    SDL_Surface *connectingSurface = TTF_RenderText_Solid(Context::font_psp2, connectingText, strlen(connectingText), textColor);
    if (!connectingSurface) {
        std::cerr << "Failed to create connecting surface: " << SDL_GetError() << std::endl;
        return;
    }

    connectingTexture = SDL_CreateTextureFromSurface(context->renderer, connectingSurface);
    if (!connectingTexture)
        std::cerr << "Failed to create connecting texture: " << SDL_GetError() << std::endl;

    SDL_DestroySurface(connectingSurface);
}

void ConnectingWindow::enterWindow() {
    state = CONNECTING_WINDOW_STATE_PENDING;

    if (!abortButton)
        abortButton = new Button(context);
    
    abortButton->setText("Abort");
    abortButton->setFocusGroup(&focusGroup);
    abortButton->setOnClickListener([this]() { onAbortClick(); });
    abortButton->setWidth();
    abortButton->queryTexture();

    if (!quitButton)
        quitButton = new Button(context);

    quitButton->setText("Quit");
    quitButton->setFocusGroup(&focusGroup);
    quitButton->setOnClickListener([this]() { onQuitClick(); });
    quitButton->setWidth();
    quitButton->queryTexture();

    std::thread connectionThread([this]() {
        try {
            context->windowManager->service->gameClient->waitConnectionBlocking();
        } catch (const NetworkInitException &ex) {
            state.store(CONNECTING_WINDOW_STATE_TIMEOUT, std::memory_order_release);
            return;
        }

        state.store(CONNECTING_WINDOW_STATE_SUCCESS, std::memory_order_release);
    });

    connectionThread.detach();

    invalidate();
    forceMotionRefresh();
}

void ConnectingWindow::leaveWindow() {
    abortButton->destroyFocus();

    if (abortButton) {
        delete abortButton;
        abortButton = nullptr;
    }

    if (quitButton) {
        delete quitButton;
        quitButton = nullptr;
    }

    if (connectingTexture) {
        SDL_DestroyTexture(connectingTexture);
        connectingTexture = nullptr;
    }
}

void ConnectingWindow::handleEvent(const SDL_Event &event) {
    Window::handleEvent(event);

    switch (event.type) {
        case SDL_EVENT_WINDOW_RESIZED:
            createConnectingTexture();
            invalidate();
            break;
    }

    bool dirty = false;

    dirty |= abortButton->handleEvents(event);
    dirty |= quitButton->handleEvents(event);

    if (dirty)
        invalidate();
}

void ConnectingWindow::compute() { 
    switch (state.load(std::memory_order_acquire)) {
        case CONNECTING_WINDOW_STATE_SUCCESS:
            context->windowManager->router->gameLoad();
            break;
        case CONNECTING_WINDOW_STATE_TIMEOUT:
            context->windowManager->service->gameClient->disconnectBlocking();
            context->windowManager->router->gameDisconnect();
            break;
    }
}

void ConnectingWindow::render() {
    if (!graphicsDirty)
        return;

    SDL_SetRenderDrawColor(context->renderer, windowColor.r, windowColor.g, windowColor.b, windowColor.a);
    SDL_RenderClear(context->renderer);

    if (!connectingTexture)
        createConnectingTexture();

    if (connectingTexture) {
        SDL_FRect dst{
            (width - connectingTexture->w) / 2.f,
            (height - connectingTexture->h) / 2.f,
            static_cast<float>(connectingTexture->w),
            static_cast<float>(connectingTexture->h)
        };
        SDL_RenderTexture(context->renderer, connectingTexture, nullptr, &dst);
    }

    int negy = height;

    if (quitButton) {
        SDL_FRect quitButtonBounds = quitButton->getBounds();
        quitButton->setPos(
            (width - quitButtonBounds.w) / 2.f,
            negy -= quitButtonBounds.h + Component::defaultMargin
        );
        quitButton->render();
    }

    if (abortButton) {
        SDL_FRect abortButtonBounds = abortButton->getBounds();
        abortButton->setPos(
            (width - abortButtonBounds.w) / 2.f,
            negy -= abortButtonBounds.h + Component::defaultMargin
        );
        abortButton->render();
    }

    SDL_RenderPresent(context->renderer);

    graphicsDirty = false;
}