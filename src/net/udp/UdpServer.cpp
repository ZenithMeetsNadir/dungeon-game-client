#include <net/udp/UdpServer.hpp>

UdpServer::UdpServer(IPv4Addr addr) {
    this->ip4.sin_family = AF_INET;
    this->ip4.sin_addr = addr.addr;
    this->ip4.sin_port = addr.port;
    this->dispatchFunc = UdpDispatchFuncT();

    if (!initWsa())
        throw WsaException();
}

UdpServer::~UdpServer() {
    deinitWsa();
}

bool UdpServer::open() {
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == INVALID_SOCKET) {
        std::cerr << "udp socket creation failed with code " << GETLASTERROR() << std::endl;
        return false;
    }

    if (!setNonBlocking(sock)) {
        closesocket(sock);
        return false;
    }

    if (bind(sock, (sockaddr *)&ip4, sizeof(ip4)) == SOCKET_ERROR) {
        std::cerr << "udp socket bind failed with code " << GETLASTERROR() << std::endl;
        closesocket(sock);
        return false;
    }

    return true;
}

void UdpServer::close() {
    if (running.load(std::memory_order_acquire)) {
        running.store(false, std::memory_order_release);
        serveTh.join();
        std::cout << "udp server shut down" << std::endl;
    }

    if (sock != INVALID_SOCKET) {
        closesocket(sock);
        sock = INVALID_SOCKET;
    }
}

bool UdpServer::handleRecvMsg(sockaddr_in srcaddr, char *buffer, int recvRes) const {
    if (!checkRecvResult(recvRes, static_cast<int>(BUFFER_SIZE)))
        return false;

    if (recvRes > 0) {
        IPv4Addr addr;
        addr.addr = srcaddr.sin_addr;
        addr.port = srcaddr.sin_port;

        dispatchFunc(this, addr, buffer, recvRes);
    }

    return true;
}

void UdpServer::listenLoop() const {
    char buffer[UdpServer::BUFFER_SIZE];

    while (running.load(std::memory_order_acquire)) {
        sockaddr_in srcaddr;
        socklen_t srcLen = sizeof(srcaddr);
        int recvRes = recvfrom(sock, buffer, UdpServer::BUFFER_SIZE, 0, (sockaddr *)&srcaddr, &srcLen);

        handleRecvMsg(srcaddr, buffer, recvRes);
    }
}

void UdpServer::listen() {
    if (!running.load(std::memory_order_acquire) && dispatchFunc) {
        running.store(true, std::memory_order_release);

        auto listenLoopBound = std::bind(listenLoop, this);
        serveTh = std::thread(listenLoopBound);

        std::cout << "udp server running..." << std::endl;
    }
}

int UdpServer::sendTo(IPv4Addr addr, const char *data, size_t size) const {
    sockaddr_in dest;
    dest.sin_family = AF_INET;
    dest.sin_addr = addr.addr;
    dest.sin_port = addr.port;

    int sendToRes = sendto(sock, data, size, 0, (sockaddr *)&dest, sizeof(dest));
    if (sendToRes == SOCKET_ERROR)
        std::cerr << "udp sendto failed with code " << GETLASTERROR() << std::endl;

    return sendToRes;
}

bool UdpServer::enableBroadcast() const {
    int broadcast = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char *)&broadcast, sizeof(broadcast)) == SOCKET_ERROR) {
        std::cerr << "enabling udp broadcast failed with code " << GETLASTERROR() << std::endl;
        return false;
    }

    return true;
}