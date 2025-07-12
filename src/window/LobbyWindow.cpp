#include <window/LobbyWindow.hpp>
#include <window/WindowManager.hpp>

const SDL_Color LobbyWindow::ServerVisual::textColor{ 255, 255, 255, 255 };

LobbyWindow::ServerVisual::ServerVisual(LobbyWindow *self, const LanLobbyClient::GameServerInfo &serverInfo)
    : serverInfo(serverInfo) 
{
    button = new SelectButton(self->context);
    button->setText(serverInfo.name + " - " + static_cast<std::string>(serverInfo.addr));
    button->setSelectGroup(&self->modeSelectGroup);
    button->setFocusGroup(&self->focusGroup);
}

LobbyWindow::ServerVisual::~ServerVisual() {
    delete button;
}

LobbyWindow::LobbyWindow(Context *context)
    : Window(context) 
{
    lanLobby = new LanLobbyClient();

    if (!lanLobby->open()) {
        std::cerr << "Failed to open LanLobbyClient" << std::endl;
        throw NetworkInitException();
    }

    serverVisuals = std::vector<ServerVisual *>();

    remoteServer = new SelectButton(context);
    remoteServer->setText("Connect to a remote server");
    remoteServer->setFocusGroup(&focusGroup);
    remoteServer->setSelectGroup(&modeSelectGroup);
    remoteServer->setWidth();
    remoteServer->queryTexture();

    remoteIp = new IpInput(context);
    remoteIp->setPlaceholder("Enter server ip <ip>:[<port>]");
    remoteIp->setFocusGroup(&focusGroup);
    remoteIp->queryTexture();
    remoteIp->lockHeight();
    remoteIp->setOnTextChangedListener([this]() { onRemoteIpTextChanged(); });

    playerName = new TextInput(context);
    playerName->setPlaceholder("Enter a name");
    playerName->setFocusGroup(&focusGroup);
    playerName->queryTexture();
    playerName->lockHeight();

    playButton = new Button(context);
    playButton->setText("Play");
    playButton->setFocusGroup(&focusGroup);
    playButton->setWidth();
    playButton->queryTexture();
    playButton->disable();
    playButton->setOnClickListener([this]() { onPlayClick(); });

    quitButton = new Button(context);
    quitButton->setText("Quit");
    quitButton->setFocusGroup(&focusGroup);
    quitButton->setOnClickListener([this]() { onQuitClick(); });
    quitButton->setWidth();
    quitButton->queryTexture();

    updateServerListDimenstions();
}

LobbyWindow::~LobbyWindow() { 
    delete quitButton;
    delete playButton;
    delete playerName;
    delete remoteIp;
    delete remoteServer;

    SDL_DestroyTexture(serverList);

    for (auto serverVisual : serverVisuals) {
        delete serverVisual;
    }

    lanLobby->close();
    delete lanLobby;
}

void LobbyWindow::onRemoteIpTextChanged() {
    try {
        playerName->setText(Dotenv::dotenv.get(("plrname_at_" + static_cast<std::string>(remoteIp->getQualfAddr())).c_str()));
    } 
    catch (const DotenvException &ex) { }
    catch (const IPv4AddrException & ex) { }
}

void LobbyWindow::onPlayClick() {
    if (modeSelectGroup && *modeSelectGroup) {
        IPv4Addr serverAddr;
        std::string server = "";

        if (remoteServer->isSelected()) {
            serverAddr = remoteIp->getQualfAddr();
            server = serverAddr;
        } else {
            for (auto &serverVisual : serverVisuals) {
                if (serverVisual->button->isSelected()) {
                    serverAddr = serverVisual->serverInfo.addr;
                    server = serverVisual->serverInfo.name;
                    break;
                }
            }
        }

        // cache player name
        Dotenv::dotenv.set(("plrname_at_" + server).c_str(), playerName->getText().c_str());

        // close LanLobbyClient (the destination address is now known)
        lanLobby->close();

        context->windowManager->service->gameClient->connectBlocking(serverAddr);
        
        context->windowManager->switchWindow(WindowManager::WindowType::game);
    }
}

void LobbyWindow::onQuitClick() {
    SDL_Event quitEvent;
    quitEvent.type = SDL_EVENT_QUIT;
    SDL_PushEvent(&quitEvent);
}

void LobbyWindow::matchServerVisuals() {
    std::vector<LanLobbyClient::GameServerInfo> listeningServers;
    lanLobby->copyListeningServers(listeningServers);

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

    playerName->setBounds(0, y, serverList->w);
    playerName->setRelPoint(serverListOffset);
    playerName->render();
    y += playerName->getBounds().h + ServerVisual::gap;

    float negy = height - ServerVisual::gap;

    SDL_FRect quitButtonBounds = quitButton->getBounds();
    quitButton->setPos(
        static_cast<float>((serverList->w - quitButtonBounds.w) / 2),
        negy -= quitButtonBounds.h
    );

    quitButton->setRelPoint(serverListOffset);
    quitButton->render();
    negy -= ServerVisual::gap;

    SDL_FRect playButtonBounds = playButton->getBounds();
    playButton->setPos(
        static_cast<float>((serverList->w - playButtonBounds.w) / 2),
        negy -= playButtonBounds.h
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
    playerName->clearVolatileState();
    playButton->clearVolatileState();
    quitButton->clearVolatileState();

    invalidateServerList();
}

void LobbyWindow::handleEvent(const SDL_Event &event) {
    Window::handleEvent(event);

    switch (event.type) {
        case SDL_EVENT_WINDOW_RESIZED:
            updateServerListDimenstions();
            break;
    }

    bool dirty = false;

    for (auto &serverVisual : serverVisuals) {
        dirty |= serverVisual->button->handleEvents(event);
    }
    
    dirty |= remoteServer->handleEvents(event);
    dirty |= remoteIp->handleEvents(event);
    dirty |= playerName->handleEvents(event);
    dirty |= playButton->handleEvents(event);
    dirty |= quitButton->handleEvents(event);

    bool lanServerSelected = false;
    for (auto &serverVisual : serverVisuals) {
        if (serverVisual->button->isSelected()) {
            lanServerSelected = true;

            // try to load cached player name  
            try {
                playerName->setText(Dotenv::dotenv.get(("plrname_at_" + serverVisual->serverInfo.name).c_str()));
            } catch (const DotenvException &ex) { }

            break;
        }
    }
    if (modeSelectGroup && *modeSelectGroup && (lanServerSelected || remoteServer->isSelected() && remoteIp->isValid()) && !playerName->getText().empty())
        dirty |= playButton->enable();
    else 
        dirty |= playButton->disable();

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

    SDL_RenderPresent(context->renderer);

    graphicsDirty = false;
}