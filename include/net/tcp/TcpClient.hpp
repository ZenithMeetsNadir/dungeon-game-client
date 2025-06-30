#ifndef TCPCLIENT_HPP
#define TCPCLIENT_HPP

#include <net/netinclude.h>
#include <net/netutil.hpp>
#include <net/IPv4Addr.hpp>
#include <functional>
#include <iostream>
#include <thread>
#include <atomic>

class TcpClient;

typedef std::function<void(const TcpClient *const, const char *, size_t)> DispatchFuncT;

class TcpClient {
    protected:
        SOCKET sock{ INVALID_SOCKET };
        sockaddr_in hostIp4;
        DispatchFuncT dispatchFunc;
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
        int sendMsg(const char *data, size_t size);
        
        void setDispatchFunc(DispatchFuncT func) {
            dispatchFunc = func;
        }
};

#endif