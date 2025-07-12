#ifndef UDPSERVER_HPP
#define UDPSERVER_HPP

#include <net/netinclude.h>
#include <net/netutil.hpp>
#include <net/IPv4Addr.hpp>
#include <iostream>
#include <thread>
#include <atomic>
#include <functional>
#include <string>
#include <exception/WsaException.hpp>

class UdpServer;

typedef std::function<void(const UdpServer *const, const IPv4Addr &, const char *, size_t)> UdpDispatchFuncT;

class UdpServer {
    protected: 
        SOCKET sock{ INVALID_SOCKET };
        sockaddr_in ip4;
        UdpDispatchFuncT dispatchFunc;
        std::thread serveTh;

        virtual void listenLoop() const;
        bool handleRecvMsg(sockaddr_in srcaddr, char *buffer, int recvRes) const;
    
    public:
        static const size_t BUFFER_SIZE = 1024;

        std::atomic_bool running = false;

        UdpServer(IPv4Addr addr);
        virtual ~UdpServer();

        virtual bool open();
        virtual void close();
        void listen();
        int sendTo(IPv4Addr addr, const char *data, size_t size) const;
        bool enableBroadcast() const;

        void setDispatchFunc(UdpDispatchFuncT func) {
            dispatchFunc = func;
        }
};

#endif