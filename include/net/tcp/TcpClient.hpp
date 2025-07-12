#ifndef TCPCLIENT_HPP
#define TCPCLIENT_HPP

#include <net/netinclude.h>
#include <net/netutil.hpp>
#include <net/IPv4Addr.hpp>
#include <functional>
#include <iostream>
#include <thread>
#include <atomic>
#include <exception/WsaException.hpp>

class TcpClient;

typedef std::function<void(const TcpClient *const, const IPv4Addr &, const char *, size_t)> TcpDispatchFuncT;

class TcpClient {
    protected:
        SOCKET sock{ INVALID_SOCKET };
        sockaddr_in hostIp4;
        TcpDispatchFuncT dispatchFunc;
        std::thread serveTh;

        void listenLoop() const;
        bool handleRecvMsg(char *buffer, int recvRes) const;

    public:
        static const size_t BUFFER_SIZE = 1024;

        std::atomic_bool running = false;

        TcpClient(IPv4Addr addr);
        ~TcpClient();

        bool open();
        void close();
        void listen();
        bool isReady() const;
        int sendMsg(const char *data, size_t size) const;
        
        void setDispatchFunc(TcpDispatchFuncT func) {
            dispatchFunc = func;
        }
};

inline bool TcpClient::isReady() const {
    return connected(sock);
}

#endif