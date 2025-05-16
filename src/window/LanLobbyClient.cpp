#include <window/LanLobbyClient.hpp>

LanLobbyClient::LanLobbyClient() {
    this->udpSearch = new UdpServer(IPv4Addr("0.0.0.0", 0));
    this->dp = new DataPacker(pw, prefix);
    this->dp->keyValueMode();
}

LanLobbyClient::~LanLobbyClient() {
    delete this->udpSearch;
    delete this->dp;
}

void LanLobbyClient::dispatchSearchResponse(const LanLobbyClient *const self, IPv4Addr addr, const char *data, size_t size) {
    std::cout << "received data from " << inet_ntoa(addr.addr) << ":" << ntohs(addr.port) << std::endl;
    
    std::string decoded = self->getDataPacker()->whichevercrypt(std::string(data, size));
    std::cout << "decoded data: " << decoded << std::endl;

    if (!self->getDataPacker()->verify(decoded)) {
        std::cerr << "invalid data received" << std::endl;
        return;
    }

    std::string serverName = self->getDataPacker()->valueOf(decoded, "sname");
    if (!serverName.empty()) {
        size_t tick = std::stoll(self->getDataPacker()->valueOf(decoded, "t"));

        std::cout << serverName << ": " << inet_ntoa(addr.addr) << std::endl;
    }
}

struct EncapsulateDispatchFunc {
    const LanLobbyClient *const self;
    
    void operator()(const UdpServer *const server, IPv4Addr addr, const char *data, size_t size) {
        LanLobbyClient::dispatchSearchResponse(self, addr, data, size);
    }
};

void LanLobbyClient::broadcastSearchLoop(const LanLobbyClient *const self) {
    IPv4Addr brdcast("255.255.255.255", 6969);
    size_t tickCounter = 0;

    std::cout << "broadcasting search trigger..." << std::endl;
    while (self->searchRunning.load(std::memory_order_acquire)) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
        
        std::string message = self->getDataPacker()->message();
        self->getDataPacker()->msgAppend(message, "s", "");
        self->getDataPacker()->msgAppend(message, "t", std::to_string(tickCounter++));

        std::string questionExistence = self->getDataPacker()->whichevercrypt(message);

        self->getUdpSearch()->sendTo(brdcast, questionExistence.c_str(), questionExistence.length());
    }
}

bool LanLobbyClient::open() {
    if (udpSearch->open() == SOCKET_ERROR) {
        std::cerr << "failed to open UDP server" << std::endl;
        return false;
    }

    udpSearch->enableBroadcast();
    udpSearch->setDispatchFunc(EncapsulateDispatchFunc{ this });

    udpSearch->listen();

    searchRunning.store(true, std::memory_order_release);
    std::thread broadcastThread(broadcastSearchLoop, this);

    std::cout << "press ENTER to stop the server..." << std::endl;
    std::cin.get();

    searchRunning.store(false, std::memory_order_release);
    broadcastThread.join();

    udpSearch->close();

    return true;
}

void LanLobbyClient::close() {
    udpSearch->close();
}