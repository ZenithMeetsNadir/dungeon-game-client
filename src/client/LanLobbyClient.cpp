#include <client/LanLobbyClient.hpp>
#include <window/Context.hpp>

LanLobbyClient::LanLobbyClient(Context *context) {
    this->context = context;

    destPort = static_cast<u_short>(std::stoi(context->service->dotenvRepo.getLanPort()));
    pw = context->service->dotenvRepo.getDefaultPassword();
    vfytkn = context->service->dotenvRepo.getDefaultVerifyToken();

    udpSearch = new UdpServer(IPv4Addr("0.0.0.0", 0));
    dp = new DataPacker(pw.c_str(), vfytkn.c_str());
    dp->keyValueMode();
    broadcastThread = std::thread();
}

LanLobbyClient::~LanLobbyClient() {
    delete dp;
    delete udpSearch;
}

void LanLobbyClient::refreshServers() {
    serversMutex.lock();
    for (size_t i = 0; i < listeningServers.size(); ++i) {
        if (listeningServers[i].tick < tickCounter - latency) {
            listeningServers.erase(listeningServers.begin() + i);
            --i;
        }
    }
    serversMutex.unlock();
}

void LanLobbyClient::refreshServers(GameServerInfo serverInfo) {
    refreshServers();

    bool updated = false;
    serversMutex.lock();
    for (size_t i = 0; i < listeningServers.size(); ++i) {
        if (listeningServers[i].sameOrigin(serverInfo)) {
            listeningServers[i] = serverInfo;
            updated = true;
        }
    }

    if (!updated)
        listeningServers.push_back(serverInfo);
        
    serversMutex.unlock();
}

void LanLobbyClient::dispatchSearchResponse(LanLobbyClient *self, IPv4Addr addr, const char *data, size_t size) {    
    std::string decoded = self->getDataPacker()->whichevercrypt(std::string(data, size));

    if (!self->getDataPacker()->verify(decoded)) {
        std::cerr << "invalid data received" << std::endl;
        return;
    }

    std::string serverName = self->getDataPacker()->valueOf(decoded, "sname");
    if (!serverName.empty()) {
        size_t tick = std::stoll(self->getDataPacker()->valueOf(decoded, "t"));

        GameServerInfo serverInfo{
            addr,
            serverName,
            tick
        };
        self->refreshServers(serverInfo);
    }
}

void LanLobbyClient::broadcastSearchLoop(LanLobbyClient *self) {
    //IPv4Addr brdcast("255.255.255.255", self->destPort);
    IPv4Addr brdcast("127.0.0.1", self->destPort);

    std::cout << "broadcasting search trigger..." << std::endl;
    while (self->searchRunning.load(std::memory_order_acquire)) {        
        std::string message = self->getDataPacker()->message();
        self->getDataPacker()->msgAppend(message, "s", "");
        self->getDataPacker()->msgAppend(message, "t", std::to_string(self->tickCounter++));

        std::string questionExistence = self->getDataPacker()->whichevercrypt(message);

        self->getUdpSearch()->sendTo(brdcast, questionExistence.c_str(), questionExistence.length());
        self->refreshServers();

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }
}

bool LanLobbyClient::open() {
    if (!udpSearch->open())
        return false;

    if (!udpSearch->enableBroadcast()) {
        udpSearch->close();
        return false;
    }

    udpSearch->setDispatchFunc(EncapsulateDispatchFunc{ this });
    udpSearch->listen();

    searchRunning.store(true, std::memory_order_release);
    broadcastThread = std::thread(broadcastSearchLoop, this);

    return true;
}

void LanLobbyClient::close() {
    if (broadcastThread.joinable()) {
        searchRunning.store(false, std::memory_order_release);
        broadcastThread.join();
    }

    udpSearch->close();
}