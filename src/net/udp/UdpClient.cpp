#include <net/udp/UdpClient.hpp>

UdpClient::UdpClient(IPv4Addr addr) : UdpServer(addr) { }

UdpClient::~UdpClient() { }

bool UdpClient::open() {
    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == INVALID_SOCKET) {
        std::cerr << "udp socket creation failed with code " << GETLASTERROR() << std::endl;
        return false;
    }

    if (!setNonBlocking(sock)) {
        closesocket(sock);
        return false;
    }

    if (connect(sock, (const sockaddr *)&ip4, sizeof(ip4)) == SOCKET_ERROR) {
        std::cerr << "udp socket connect failed with code " << GETLASTERROR() << std::endl;
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
        std::cerr << "udp send failed with code " << GETLASTERROR() << std::endl;

    return sendRes;
}