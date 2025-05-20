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
    updateServerListTexture();
}

LobbyWindow::~LobbyWindow() { 
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
        }
    }

    for (const auto &serverInfo : listeningServers) {
        bool found = false;
        for (auto serverVisual : serverVisuals) {
            if (serverVisual->serverInfo.sameOrigin(serverInfo))
                found = true;
        }

        if (!found)
            serverVisuals.push_back(new ServerVisual(this, serverInfo));
    }
}

void LobbyWindow::invalidateServerVisuals() {
    for (auto serverVisual : serverVisuals) {
        if (serverVisual->texture) {
            SDL_DestroyTexture(serverVisual->texture);
            serverVisual->texture = nullptr;
        }
    }
}

void LobbyWindow::updateServerVisuals() {
    for (auto serverVisual : serverVisuals) {
        if (!serverVisual->texture)
            serverVisual->texture = createServerVisual(serverVisual->serverInfo);
    }
}

SDL_Texture *LobbyWindow::createServerVisual(const LanLobbyClient::GameServerInfo &serverInfo) const {
    std::string text = serverInfo.name + " - " + static_cast<std::string>(serverInfo.addr);
    SDL_Surface *surface = TTF_RenderText_Solid(
        font, 
        text.c_str(), 
        text.length(), 
        ServerVisual::textColor
    );

    SDL_Texture *texture = SDL_CreateTextureFromSurface(context->renderer, surface);
    if (!texture)
        std::cout << "Failed to create texture from surface: " << SDL_GetError() << std::endl;

    SDL_DestroySurface(surface);

    return texture;
}

void LobbyWindow::updateServerListTexture() {
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
        SDL_FRect box{
            0, 
            y, 
            static_cast<float>(serverList->w), 
            static_cast<float>(serverVisual->texture->h + 2 * ServerVisual::padding)
        };

        serverVisual->box = box;         

        if (serverVisual->hovered) {
            if (mousePressed)
                SDL_SetRenderDrawColor(context->renderer, 100, 100, 150, 255);
            else
                SDL_SetRenderDrawColor(context->renderer, 80, 80, 120, 255);
        } else
            SDL_SetRenderDrawColor(context->renderer, 60, 60, 60, 255);

        SDL_RenderFillRect(context->renderer, &box);

        SDL_FRect textRect{
            ServerVisual::padding, 
            y + ServerVisual::padding, 
            static_cast<float>(serverVisual->texture->w), 
            static_cast<float>(serverVisual->texture->h)
        };

        SDL_RenderTexture(context->renderer, serverVisual->texture, nullptr, &textRect);

        y += box.h + ServerVisual::gap;
    }

    SDL_SetRenderTarget(context->renderer, nullptr);
}

void LobbyWindow::handleHover() {
    float mx, my;
    SDL_GetMouseState(&mx, &my);

    SDL_FPoint serverListOffset = getServerListOffset();
    SDL_FPoint relativeMPos{
        mx - serverListOffset.x,
        my - serverListOffset.y
    };

    for (auto serverVisual : serverVisuals) {
        serverVisual->hovered = SDL_PointInRectFloat(&relativeMPos, &serverVisual->box);
    }
}

void LobbyWindow::handleEvent(const SDL_Event &event) {
    Window::handleEvent(event);

    switch (event.type) {
        case SDL_EVENT_WINDOW_RESIZED:
            updateServerListTexture();
            invalidateServerVisuals();
            updateServerVisuals();
            break;
        case SDL_EVENT_MOUSE_BUTTON_DOWN:
            if (event.button.button == SDL_BUTTON_LEFT)
                mousePressed = true;
            break;
        case SDL_EVENT_MOUSE_BUTTON_UP:
            if (event.button.button == SDL_BUTTON_LEFT)
                mousePressed = false;
            break;
    }
}

void LobbyWindow::render() {
    matchServerVisuals();
    updateServerVisuals();
    handleHover();
    prepareServerList();

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

    SDL_RenderPresent(context->renderer);
}