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
    updateServerListDimenstions();
    
    playButtonComponent = new Button(context->renderer, "Play");
    playButtonComponent->setBounds();
    playButtonComponent->queryTexture();
}

LobbyWindow::~LobbyWindow() { 
    delete playButtonComponent;

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
}

void LobbyWindow::prepareServerList() const {
    SDL_SetRenderTarget(context->renderer, serverList);
    SDL_SetRenderDrawColor(context->renderer, 0, 0, 0, 0);
    SDL_RenderClear(context->renderer);

    float y = ServerVisual::gap;

    for (auto serverVisual : serverVisuals) {
        serverVisual->button->setBounds(0, y, serverList->w);

        serverVisual->button->setRelPoint(getServerListOffset());
        serverVisual->button->render();

        y += serverVisual->button->getBounds().h + ServerVisual::gap;
    }

    SDL_FRect playButtonBounds = playButtonComponent->getBounds();
    playButtonComponent->setPos(
        static_cast<float>((serverList->w - playButtonBounds.w) / 2),
        height - playButtonBounds.h - ServerVisual::gap
    );

    playButtonComponent->setRelPoint(getServerListOffset());
    playButtonComponent->render();

    SDL_SetRenderTarget(context->renderer, nullptr);
}

void LobbyWindow::handleEvent(const SDL_Event &event) {
    Window::handleEvent(event);

    switch (event.type) {
        case SDL_EVENT_WINDOW_RESIZED:
            updateServerListDimenstions();
            break;
    }

    for (auto serverVisual : serverVisuals) {
        serverVisual->button->handleMouseEvents(event);
    }

    playButtonComponent->handleMouseEvents(event);

    invalidateServerList();
}

void LobbyWindow::render() {
    matchServerVisuals();

    if (serverListDirty) {
        prepareServerList();
        serverListDirty = false;
    }

    SDL_SetRenderDrawColor(context->renderer, 30, 30, 30, 255);
    SDL_RenderClear(context->renderer);

    SDL_FPoint serverListOffset = getServerListOffset();
    SDL_FRect box{
        serverListOffset.x,
        serverListOffset.y,
        static_cast<float>(serverList->w), 
        static_cast<float>(serverList->h)
    };

    SDL_RenderTexture(context->renderer, serverList, nullptr, &box);

    /*SDL_Texture* tex = IMG_LoadTexture(context->renderer, "C:\\Users\\marti\\programy\\angular\\pisq\\pisqapp\\src\\assets\\images\\debris.png");
    if (!tex) 
        std::cerr << "Failed to load texture: " << SDL_GetError() << "\n";

    SDL_RenderTexture(context->renderer, tex, nullptr, &box);
    SDL_DestroyTexture(tex);*/

    SDL_RenderPresent(context->renderer);
}