#ifndef UDPSERVER_HPP
#define UDPSERVER_HPP

#ifdef _WIN32
    #include <WinSock2.h>

    #define WOULDBLOCK WSAEWOULDBLOCK
    #define MSGTOOBIG WSAEMSGSIZE

    #define GETLASTERROR() WSAGetLastError()

    typedef int socklen_t;
#endif
#ifdef __linux__
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <fcntl.h>

    #define INVALID_SOCKET (-1)
    #define SOCKET_ERROR (-1)
    #define WOULDBLOCK EWOULDBLOCK
    #define MSGTOOBIG 0

    #define GETLASTERROR() errno

    inline int closesocket(int sockfd) { return close(sockfd); }

    typedef int SOCKET;
#endif

#include <iostream>
#include <thread>
#include <atomic>
#include <functional>
#include <string>

struct IPv4Addr;
class UdpServer;

typedef std::function<void(const UdpServer *const, IPv4Addr, const char *, size_t)> DispatchFuncT;

struct IPv4Addr {
    in_addr addr;
    u_short port;

    IPv4Addr();
    IPv4Addr(const char *addr, u_short port);

    bool operator==(const IPv4Addr &other) const {
        return addr.s_addr == other.addr.s_addr && port == other.port;
    }

    operator std::string() const {
        return std::string(inet_ntoa(this->addr)) + ":" + std::to_string(ntohs(this->port));
    }
};

class UdpServer {
    protected: 
        SOCKET sock{ INVALID_SOCKET };
        sockaddr_in ip4;
        DispatchFuncT dispatchFunc;
        std::thread serveTh;

        SOCKET getNonBlockingDGram();

        virtual void listenLoop() const;
        bool handleRecvMsg(sockaddr_in srcaddr, char *buffer, int recvRes) const;
    
    public:
        static const size_t BUFFER_SIZE = 1024;

        std::atomic_bool running = false;

        UdpServer(IPv4Addr addr);
        virtual ~UdpServer();

        virtual bool open();
        virtual void close();
        virtual void listen();
        int sendTo(IPv4Addr addr, const char *data, size_t size) const;
        bool enableBroadcast() const;

        void setDispatchFunc(DispatchFuncT func) {
            dispatchFunc = func;
        }
};

#endif