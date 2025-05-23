#include <client/LanLobbyClient.hpp>

const char *const LanLobbyClient::pw = "nejtajnejsiheslouwu";
const char *const LanLobbyClient::prefix = "uwu";

LanLobbyClient::LanLobbyClient() {
    this->udpSearch = new UdpServer(IPv4Addr("0.0.0.0", 0));
    this->dp = new DataPacker(pw, prefix);
    this->dp->keyValueMode();
}

LanLobbyClient::~LanLobbyClient() {
    delete this->udpSearch;
    delete this->dp;
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
    //std::cout << "received data from " << inet_ntoa(addr.addr) << ":" << ntohs(addr.port) << std::endl;
    
    std::string decoded = self->getDataPacker()->whichevercrypt(std::string(data, size));
    //std::cout << "decoded data: " << decoded << std::endl;

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

        //std::cout << serverName << ": " << inet_ntoa(addr.addr) << std::endl;
    }
}

struct EncapsulateDispatchFunc {
    LanLobbyClient *self;
    
    void operator()(const UdpServer *const server, IPv4Addr addr, const char *data, size_t size) {
        LanLobbyClient::dispatchSearchResponse(self, addr, data, size);
    }
};

void LanLobbyClient::broadcastSearchLoop(LanLobbyClient *self) {
    IPv4Addr brdcast("255.255.255.255", 6969);

    std::cout << "broadcasting search trigger..." << std::endl;
    while (self->searchRunning.load(std::memory_order_acquire)) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        
        std::string message = self->getDataPacker()->message();
        self->getDataPacker()->msgAppend(message, "s", "");
        self->getDataPacker()->msgAppend(message, "t", std::to_string(self->tickCounter++));

        std::string questionExistence = self->getDataPacker()->whichevercrypt(message);

        self->getUdpSearch()->sendTo(brdcast, questionExistence.c_str(), questionExistence.length());
        self->refreshServers();
    }
}

bool LanLobbyClient::open() {
    if (!udpSearch->open())
        return false;

    if (!udpSearch->enableBroadcast())
        return false;

    udpSearch->setDispatchFunc(EncapsulateDispatchFunc{ this });

    udpSearch->listen();

    searchRunning.store(true, std::memory_order_release);
    broadcastThread = std::thread(broadcastSearchLoop, this);

    return true;
}

void LanLobbyClient::close() {
    searchRunning.store(false, std::memory_order_release);
    broadcastThread.join();

    udpSearch->close();
}