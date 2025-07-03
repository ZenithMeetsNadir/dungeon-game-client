#ifndef LANLOBBY_HPP
#define LANLOBBY_HPP

#include <iostream>
#include <thread>
#include <atomic>
#include <vector>
#include <mutex>
#include <net/udp/UdpServer.hpp>
#include <net/DataPacker.hpp>
#include <util/Dotenv.hpp>

class LanLobbyClient {
    public:
        struct GameServerInfo;

    protected:
        u_short destPort;
        std::string pw;
        std::string vfytkn;

        UdpServer *udpSearch;
        DataPacker *dp;

        std::thread broadcastThread;

        std::vector<GameServerInfo> listeningServers;

        void refreshServers();
        void refreshServers(GameServerInfo serverInfo);

        static void broadcastSearchLoop(LanLobbyClient *self);

    public:
        struct GameServerInfo {
            IPv4Addr addr;
            std::string name;
            size_t tick;

            bool operator==(const GameServerInfo &other) const {
                return addr == other.addr && name == other.name && tick == other.tick;
            }

            bool sameOrigin(const GameServerInfo &other) const {
                return addr == other.addr && name == other.name;
            }
        };

        size_t tickCounter{ 0 };
        std::atomic_bool searchRunning{ false };
        std::mutex serversMutex;

        static const size_t latency = 3;

        LanLobbyClient();
        ~LanLobbyClient();

        bool open();
        void close();

        DataPacker *getDataPacker() const {
            return dp;
        }
        UdpServer *getUdpSearch() const {
            return udpSearch;
        }
        void copyListeningServers(std::vector<GameServerInfo> &dest) {
            serversMutex.lock();
            dest = listeningServers;    
            serversMutex.unlock();
        }

        static void dispatchSearchResponse(LanLobbyClient *self, IPv4Addr addr, const char *data, size_t size);
};

#endif