#ifndef GAMECLIENT_HPP
#define GAMECLIENT_HPP

#include <net/udp/UdpClient.hpp>
#include <net/tcp/TcpClient.hpp>
#include <net/DataPacker.hpp>
#include <net/IPv4Addr.hpp>
#include <util/Dotenv.hpp>
#include <vector>
#include <string>
#include <functional>
#include <atomic>
#include <exception/DataPackerException.hpp>
#include <exception/NetworkException.hpp>

class Context;

typedef std::function<void(const IPv4Addr &, const char *, size_t)> DispatchHandlerT;

class GameClient {
    protected:
        struct EncapsulateDispatchFunc {
            GameClient *self;

            void operator()(const UdpServer *const, const IPv4Addr &addr, const char *data, size_t size) {
                self->dispatchUdpMsg(addr, data, size);
            }

            void operator()(const TcpClient *const, const IPv4Addr &addr, const char *data, size_t size) {
                self->dispatchTcpMsg(addr, data, size);
            }
        };

        Context *context;

        UdpClient *udpClient{ nullptr };
        TcpClient *tcpClient{ nullptr };
        DataPacker *dp;

        std::string serverPw;
        std::string serverVfytkn;

        std::vector<DispatchHandlerT *> dispatchUdpHandlers;
        std::vector<DispatchHandlerT *> dispatchTcpHandlers;

    public:
        GameClient(Context *context);
        ~GameClient();

        bool open(const IPv4Addr &serverAddr);
        void waitConnectionBlocking();
        void notifyDisconnectBlocking();
        void close();

        void openConnect(const IPv4Addr &serverAddr);
        void connectBlocking(const IPv4Addr &serverAddr);
        void disconnectBlocking();

        bool tcpIsReady();
        void sendUdpMsg(const char *data);
        void sendTcpMsg(const char *data);
        void dispatchUdpMsg(const IPv4Addr &addr, const char *data, size_t size);
        void dispatchTcpMsg(const IPv4Addr &addr, const char *data, size_t size);

        void addDispatchUdpHandler(DispatchHandlerT *handler);
        void addDispatchTcpHandler(DispatchHandlerT *handler);
        void removeDispatchUdpHandler(DispatchHandlerT *rmHandler);
        void removeDispatchTcpHandler(DispatchHandlerT *rmHandler);
};

inline bool GameClient::tcpIsReady() {
    return tcpClient && tcpClient->isReady();
}

#endif