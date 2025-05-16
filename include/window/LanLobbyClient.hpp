#ifndef LANLOBBY_HPP
#define LANLOBBY_HPP

#include <iostream>
#include <thread>
#include <atomic>
#include <vector>
#include <mutex>
#include <udp/UdpServer.hpp>
#include <udp/DataPacker.hpp>

const u_short destPort = 6969;
const char *const pw = "nejtajnejsiheslouwu";
const char *const prefix = "uwu";
const size_t latency = 5;

struct GameServerInfo {
    IPv4Addr addr;
    std::string name;
    size_t tick;
};

class LanLobbyClient {
    protected:
        UdpServer *udpSearch;
        DataPacker *dp;

        std::thread broadcastThread;

        std::vector<GameServerInfo> listeningServers;

        static void broadcastSearchLoop(LanLobbyClient *self);

    public:
        size_t tickCounter{ 0 };
        std::atomic_bool searchRunning{ false };
        std::mutex serversMutex;

        LanLobbyClient();
        ~LanLobbyClient();

        bool open();
        void close();

        void refreshServers();
        void refreshServers(GameServerInfo serverInfo);

        DataPacker *getDataPacker() const {
            return dp;
        }
        UdpServer *getUdpSearch() const {
            return udpSearch;
        }
        std::vector<GameServerInfo> getListeningServers() const {
            return listeningServers;
        }

        static void dispatchSearchResponse(LanLobbyClient *self, IPv4Addr addr, const char *data, size_t size);
};

#endif