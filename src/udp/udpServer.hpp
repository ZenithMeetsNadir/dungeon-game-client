#ifndef UDPSERVER_HPP
#define UDPSERVER_HPP

#include <WinSock2.h>
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
        std::atomic_bool running = false;
        std::thread serveTh;

        SOCKET getNonBlockingDGram();
    
    public:
        static const size_t BUFFER_SIZE = 1024;

        UdpServer(IPv4Addr addr);
        ~UdpServer();

        int open();
        void close();
        void listenLoop() const;
        void listen();
        int sendTo(IPv4Addr addr, const char *data, size_t size) const;
        void enableBroadcast() const;
        
        inline void setDispatchFunc(DispatchFuncT func) {
            dispatchFunc = func;
        }
};

#endif