#include <window/LobbyWindow.hpp>

const SDL_Color LobbyWindow::ServerVisual::textColor = { 255, 255, 255, 255 };

LobbyWindow::LobbyWindow(Context *context, TTF_Font *font)
    : Window(context, font) 
{
    width = context->initWidth;
    height = context->initHeight;

    lanLobby = new LanLobbyClient();

    if (!lanLobby->open()) {
        std::cerr << "Failed to open UDP server" << std::endl;
        lanLobby->close();
    }

    serverVisuals = std::vector<ServerVisual *>();

    remoteServer = new SelectButton(context);
    remoteServer->setText("Connect to a remote server");
    remoteServer->setFocusGroup(&focusGroup);
    remoteServer->setSelectGroup(&modeSelectGroup);
    remoteServer->setWidth();
    remoteServer->queryTexture();

    remoteIp = new TextInput(context);
    remoteIp->setPlaceholder("Enter server ip (ip:port)");
    remoteIp->setFocusGroup(&focusGroup);
    remoteIp->queryTexture();
    remoteIp->lockHeight();

    playButton = new Button(context);
    playButton->setText("Play");
    playButton->setFocusGroup(&focusGroup);
    playButton->setWidth();
    playButton->queryTexture();

    pauseOverlay = new PauseOverlay(context);
    pauseOverlay->setControlsFocusGroup(&focusGroup);

    updateServerListDimenstions();
}

LobbyWindow::~LobbyWindow() { 
    delete playButton;
    delete remoteIp;
    delete remoteServer;

    SDL_DestroyTexture(serverList);

    for (auto serverVisual : serverVisuals) {
        delete serverVisual;
    }

    lanLobby->close();
    delete lanLobby;
}

void LobbyWindow::matchServerVisuals() {
    std::vector<LanLobbyClient::GameServerInfo> listeningServers;
    lanLobby->copyListeningServersLock(listeningServers);

    for (size_t i = 0; i < serverVisuals.size(); ++i) {
        bool found = false;
        for (const auto &serverInfo : listeningServers) {
            if (serverVisuals[i]->serverInfo.sameOrigin(serverInfo)) {
                found = true;
            }
        }

        if (!found) {
            auto removed = serverVisuals.erase(serverVisuals.begin() + i);
            delete removed[0];
            --i;
            invalidateServerList();
        }
    }

    for (const auto &serverInfo : listeningServers) {
        bool found = false;
        for (auto serverVisual : serverVisuals) {
            if (serverVisual->serverInfo.sameOrigin(serverInfo))
                found = true;
        }

        if (!found) {
            serverVisuals.push_back(new ServerVisual(this, serverInfo));
            invalidateServerList();
        }
    }
}

void LobbyWindow::updateServerListDimenstions() {
    serverList = SDL_CreateTexture(
        context->renderer, 
        SDL_PIXELFORMAT_RGBA8888, 
        SDL_TEXTUREACCESS_TARGET, 
        ServerVisual::width, height
    );

    SDL_FRect pauseOverlayBounds{
        0, 0,
        static_cast<float>(width),
        static_cast<float>(height)
    };
    pauseOverlay->setBounds(pauseOverlayBounds);

    invalidateServerList();
    invalidate();
}

void LobbyWindow::prepareServerList() const {
    SDL_Texture *oldTarget = SDL_GetRenderTarget(context->renderer);
    SDL_SetRenderTarget(context->renderer, serverList);
    SDL_SetRenderDrawColor(context->renderer, 0, 0, 0, 0);
    SDL_RenderClear(context->renderer);

    float y = ServerVisual::gap;
    SDL_FPoint serverListOffset = getServerListOffset();

    for (auto &serverVisual : serverVisuals) {
        serverVisual->button->setBounds(0, y, serverList->w);
        serverVisual->button->setRelPoint(serverListOffset);
        serverVisual->button->render();
        y += serverVisual->button->getBounds().h + ServerVisual::gap;
    }

    remoteServer->setBounds(0, y, serverList->w);
    remoteServer->setRelPoint(serverListOffset);
    remoteServer->render();
    y += remoteServer->getBounds().h + ServerVisual::gap;

    bool remoteSelected = remoteServer->isSelected();
    if (remoteSelected != remoteIp->isAttached())
        remoteSelected ? remoteIp->attach() : remoteIp->detach();

    remoteIp->setBounds(0, y, serverList->w);
    remoteIp->setRelPoint(serverListOffset);
    remoteIp->render();
    y += remoteSelected ? remoteIp->getBounds().h + ServerVisual::gap : 0;

    SDL_FRect playButtonBounds = playButton->getBounds();
    playButton->setPos(
        static_cast<float>((serverList->w - playButtonBounds.w) / 2),
        height - playButtonBounds.h - ServerVisual::gap
    );

    playButton->setRelPoint(serverListOffset);
    playButton->render();

    SDL_SetRenderTarget(context->renderer, oldTarget);
}

void LobbyWindow::clearLobbyVolatileState() {
    for (auto &serverVisual : serverVisuals) {
        serverVisual->button->clearVolatileState();
    }

    remoteServer->clearVolatileState();
    remoteIp->clearVolatileState();
    playButton->clearVolatileState();

    invalidateServerList();
}

void LobbyWindow::forceMotionRefresh() {
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

    handleEvent(event);
}

void LobbyWindow::handleEvent(const SDL_Event &event) {
    Window::handleEvent(event);

    switch (event.type) {
        case SDL_EVENT_WINDOW_RESIZED:
            updateServerListDimenstions();
            break;
    }

    bool dirty = false;

    dirty |= pauseOverlay->handleEvents(event);

    if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_ESCAPE) {
        pauseOverlay->isAttached() ? pauseOverlay->detach() : pauseOverlay->attach();

        if (pauseOverlay->isAttached()) {
            clearLobbyVolatileState();
            pauseOverlay->clearVolatileState();
        }     

        forceMotionRefresh();
        dirty |= true;
    }

    if (dirty)
        invalidate();

    dirty = false;

    if (!pauseOverlay->isAttached()) {
        for (auto &serverVisual : serverVisuals) {
            dirty |= serverVisual->button->handleEvents(event);
        }

        dirty |= remoteServer->handleEvents(event);
        dirty |= remoteIp->handleEvents(event);
        dirty |= playButton->handleEvents(event);
    }

    if (dirty)
        invalidateServerList();    
}

void LobbyWindow::render() {
    if (SDL_GetTicks() % 1000 == 0)
        matchServerVisuals();

    if (!graphicsDirty)
        return;

    if (serverListDirty) {
        prepareServerList();
        serverListDirty = false;
    }

    SDL_SetRenderDrawColor(context->renderer, windowColor.r, windowColor.g, windowColor.b, windowColor.a);
    SDL_RenderClear(context->renderer);

    SDL_FPoint serverListOffset = getServerListOffset();
    SDL_FRect box{
        serverListOffset.x,
        serverListOffset.y,
        static_cast<float>(serverList->w), 
        static_cast<float>(serverList->h)
    };

    SDL_RenderTexture(context->renderer, serverList, nullptr, &box);

    pauseOverlay->render();

    /*SDL_Texture* tex = IMG_LoadTexture(context->renderer, "C:\\Users\\marti\\programy\\angular\\pisq\\pisqapp\\src\\assets\\images\\debris.png");
    if (!tex) 
        std::cerr << "Failed to load texture: " << SDL_GetError() << "\n";

    SDL_RenderTexture(context->renderer, tex, nullptr, &box);
    SDL_DestroyTexture(tex);*/

    SDL_RenderPresent(context->renderer);

    graphicsDirty = false;
}