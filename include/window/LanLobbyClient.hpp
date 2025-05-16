#ifndef LANLOBBY_HPP
#define LANLOBBY_HPP

#include <iostream>
#include <thread>
#include <atomic>
#include <vector>
#include <udp/UdpServer.hpp>
#include <udp/DataPacker.hpp>

const u_short destPort = 6969;
const char *const pw = "nejtajnejsiheslouwu";
const char *const prefix = "uwu";

struct GameServerInfo {

};

class LanLobbyClient {
    protected:
        UdpServer *udpSearch;
        DataPacker *dp;

        std::thread broadcastThread;

        std::vector<GameServerInfo> listeningServers;

        static void broadcastSearchLoop(const LanLobbyClient *const self);

    public:
        std::atomic_bool searchRunning = false;

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

        static void dispatchSearchResponse(const LanLobbyClient *const self, IPv4Addr addr, const char *data, size_t size);
};

#endif