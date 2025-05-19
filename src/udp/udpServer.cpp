#include <udp/UdpServer.hpp>

IPv4Addr::IPv4Addr() {
    this->addr.s_addr = INADDR_ANY;
    this->port = htons(0);
}

IPv4Addr::IPv4Addr(const char *addr, u_short port) {
    this->addr.s_addr = inet_addr(addr);
    this->port = htons(port);
}

UdpServer::UdpServer(IPv4Addr addr) {
    this->ip4.sin_family = AF_INET;
    this->ip4.sin_addr = addr.addr;
    this->ip4.sin_port = addr.port;
    this->dispatchFunc = DispatchFuncT();

    #ifdef _WIN32
        WSAData wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData)) {
            std::cerr << "WSAStartup failed" << std::endl;
            return;
        }
    #endif
}

UdpServer::~UdpServer() {
    #ifdef _WIN32
        WSACleanup();
    #endif
}

SOCKET UdpServer::getNonBlockingDGram() {
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == INVALID_SOCKET) {
        std::cerr << "socket creation failed with code " << GETLASTERROR() << std::endl;
        closesocket(sock);
        return INVALID_SOCKET;
    }

    #ifdef _WIN32
        u_long nonblocking = 1;

        if (ioctlsocket(sock, FIONBIO, &nonblocking) == SOCKET_ERROR) {
            std::cerr << "Setting socket to non-blocking failed with code " << GETLASTERROR() << std::endl;
            closesocket(sock);
            return INVALID_SOCKET;
        }
    #endif
    #ifdef __linux__
        int flags = fcntl(sock, F_GETFL);

        if (fcntl(sock, F_SETFL, flags | O_NONBLOCK) == SOCKET_ERROR) {
            std::cerr << "Setting socket to non-blocking failed with code " << GETLASTERROR() << std::endl;
            closesocket(sock);
            return INVALID_SOCKET;
        }
    #endif

    return sock;
}

bool UdpServer::open() {
    sock = getNonBlockingDGram();
    if (sock == INVALID_SOCKET) {
        std::cerr << "socket creation failed" << std::endl;
        return false;
    }

    if (bind(sock, (sockaddr *)&ip4, sizeof(ip4)) == SOCKET_ERROR) {
        std::cerr << "socket bind failed with code " << GETLASTERROR() << std::endl;
        closesocket(sock);
        return false;
    }

    return true;
}

void UdpServer::close() {
    if (running.load(std::memory_order_acquire)) {
        running.store(false, std::memory_order_release);
        serveTh.join();
        std::cout << "server shut down" << std::endl;
    }

    if (sock != INVALID_SOCKET)
        closesocket(sock);

    sock = INVALID_SOCKET;
}

bool UdpServer::handleRecvMsg(sockaddr_in srcaddr, char *buffer, int recvRes) const {
    if (recvRes == SOCKET_ERROR) {
        int err = GETLASTERROR();
        if (err == MSGTOOBIG)
            recvRes = UdpServer::BUFFER_SIZE;
        else {
            if (err != WOULDBLOCK)
                std::cerr << "recv failed with code " << err << std::endl;
        
            return false;
        }
    }

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

        auto listenLoopBound = std::bind(&UdpServer::listenLoop, this);
        serveTh = std::thread(listenLoopBound);

        std::cout << "sever running..." << std::endl;
    }
}

int UdpServer::sendTo(IPv4Addr addr, const char *data, size_t size) const {
    sockaddr_in dest;
    dest.sin_family = AF_INET;
    dest.sin_addr = addr.addr;
    dest.sin_port = addr.port;

    int sendToRes = sendto(sock, data, size, 0, (sockaddr *)&dest, sizeof(dest));
    if (sendToRes == SOCKET_ERROR)
        std::cerr << "sendto failed with code " << GETLASTERROR() << std::endl;

    return sendToRes;
}

bool UdpServer::enableBroadcast() const {
    int broadcast = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (char *)&broadcast, sizeof(broadcast)) == SOCKET_ERROR) {
        std::cerr << "enabling broadcast failed with code " << GETLASTERROR() << std::endl;
        return false;
    }

    return true;
}