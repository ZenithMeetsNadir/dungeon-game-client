#include <net/tcp/TcpClient.hpp>

TcpClient::TcpClient(IPv4Addr addr) {
    hostIp4.sin_family = AF_INET;
    hostIp4.sin_addr = addr.addr;
    hostIp4.sin_port = addr.port;
    dispatchFunc = TcpDispatchFuncT();

    if (!initWsa())
        throw WsaException();
}

TcpClient::~TcpClient() {
    deinitWsa();
}

bool TcpClient::open() {
    sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        std::cerr << "tcp socket creation failed with code " << GETLASTERROR() << std::endl;
        return false;
    }

    if (!setNonBlocking(sock)) {
        closesocket(sock);
        return false;
    }

    if (connect(sock, (const sockaddr *)&hostIp4, sizeof(hostIp4)) == SOCKET_ERROR) {
        int err = GETLASTERROR();
        if (err != CONNECTWOULDBLOCK) {
            std::cerr << "tcp socket connect failed with code " << err << std::endl;
            closesocket(sock);
            return false;
        }
    }

    return true;
}

void TcpClient::close() {
    if (running.load(std::memory_order_acquire)) {
        running.store(false, std::memory_order_release);
        serveTh.join();
        std::cout << "tcp server shut down" << std::endl;
    }

    if (sock != INVALID_SOCKET)
        closesocket(sock);

    sock = INVALID_SOCKET;
}

bool TcpClient::handleRecvMsg(char *buffer, int recvRes) const {
    if (!checkRecvResult(recvRes, static_cast<int>(BUFFER_SIZE)))
        return false;

    IPv4Addr addr;
    addr.addr = hostIp4.sin_addr;
    addr.port = hostIp4.sin_port;
    if (recvRes > 0)
        dispatchFunc(this, addr, buffer, recvRes);

    return true;
}

void TcpClient::listenLoop() const {
    char buffer[BUFFER_SIZE];

    while (running.load(std::memory_order_acquire)) {
        int recvRes = recv(sock, buffer, BUFFER_SIZE, 0);

        if (!handleRecvMsg(buffer, recvRes))
            continue;
    }
}

void TcpClient::listen() {
    if (!running.load(std::memory_order_acquire)) {
        running.store(true, std::memory_order_release);

        auto listenLoopBound = std::bind(&TcpClient::listenLoop, this);
        serveTh = std::thread(listenLoopBound);

        std::cout << "tcp server running..." << std::endl;
    }
}

int TcpClient::sendMsg(const char *data, size_t size) const {
    int sendRes = send(sock, data, static_cast<int>(size), 0);
    if (sendRes == SOCKET_ERROR)
        std::cerr << "tcp send failed with code " << GETLASTERROR() << std::endl;

    return sendRes;
}