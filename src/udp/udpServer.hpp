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

struct IPv4Addr;
class UdpServer;

typedef std::function<void(const UdpServer *const, IPv4Addr, const char *, size_t)> DispatchFuncT;

struct IPv4Addr {
    in_addr addr;
    u_short port;

    IPv4Addr();
    IPv4Addr(const char *addr, u_short port);
};

class UdpServer {
    protected: 
        SOCKET sock;
        sockaddr_in ip4;
        DispatchFuncT dispatchFunc;
        std::thread serveTh;

        SOCKET getNonBlockingDGram();
    
    public:
        static const size_t BUFFER_SIZE = 1024;

        std::atomic_bool running = false;

        UdpServer(IPv4Addr addr);
        ~UdpServer();

        int open();
        void close();
        void listen();
        int sendTo(IPv4Addr addr, const char *data, size_t size) const;
        void enableBroadcast() const;
        
        inline SOCKET getSocket() const {
            return sock;
        }

        inline sockaddr_in getIp4() const {
            return ip4;
        }

        inline DispatchFuncT getDispatchFunc() const {
            return dispatchFunc;
        }

        inline void setDispatchFunc(DispatchFuncT func) {
            dispatchFunc = func;
        }
};

#endif