#include <udp/UdpClient.hpp>

UdpClient::UdpClient(IPv4Addr addr) : UdpServer(addr) { }

UdpClient::~UdpClient() { }

bool UdpClient::open() {
    sock = getNonBlockingDGram();
    if (sock == INVALID_SOCKET) {
        std::cerr << "socket creation failed" << std::endl;
        return false;
    }

    if (connect(sock, (sockaddr *)&ip4, sizeof(ip4)) == SOCKET_ERROR) {
        std::cerr << "socket connect failed with code " << GETLASTERROR() << std::endl;
        closesocket(sock);
        return false;
    }

    return true;
}

void UdpClient::listenLoop() const {
    char buffer[UdpServer::BUFFER_SIZE];

    while (running.load(std::memory_order_acquire)) {
        int recvRes = recv(sock, buffer, UdpServer::BUFFER_SIZE, 0);
        handleRecvMsg(ip4, buffer, recvRes);
    }
}

int UdpClient::sendMsg(const char *data, size_t size) const {
    int sendRes = send(sock, data, size, 0);
    if (sendRes == SOCKET_ERROR)
        std::cerr << "send failed with code " << GETLASTERROR() << std::endl;

    return sendRes;
}