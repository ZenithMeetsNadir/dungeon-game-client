#include <net/netutil.hpp>

bool initWsa() {
    #ifdef _WIN32
        WSAData wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData)) {
            std::cerr << "WSAStartup failed" << std::endl;
            return false;
        }
    #endif

    return true;
}

void deinitWsa() {
    #ifdef _WIN32
        WSACleanup();
    #endif
}

bool setNonBlocking(SOCKET sock) {
    #ifdef _WIN32
        u_long nonblocking = 1;

        if (ioctlsocket(sock, FIONBIO, &nonblocking) == SOCKET_ERROR) {
            std::cerr << "setting socket to non-blocking failed with code " << GETLASTERROR() << std::endl;
            return false;
        }

        return true;
    #endif
    #ifdef __linux__
        int flags = fcntl(sock, F_GETFL);

        if (fcntl(sock, F_SETFL, flags | O_NONBLOCK) == SOCKET_ERROR) {
            std::cerr << "setting socket to non-blocking failed with code " << GETLASTERROR() << std::endl;
            return false;
        }

        return true;
    #endif

    return false;
}

bool checkRecvResult(int &recvRes, int bufferSize) {
    if (recvRes == SOCKET_ERROR) {
        int err = GETLASTERROR();
        switch (err) {
            case MSGTOOBIG:
                recvRes = bufferSize;
                break;
            case WOULDBLOCK:
                recvRes = 0;
                break;
            default:
                std::cerr << "udp recv failed with code " << err << std::endl;
                return false;
        }
    }

    return true;
}
