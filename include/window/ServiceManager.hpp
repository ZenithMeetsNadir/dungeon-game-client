#ifndef SERVICEMANAGER_HPP
#define SERVICEMANAGER_HPP

#include <client/GameClient.hpp>

struct ServiceManager {
    GameClient *gameClient;

    ServiceManager();
    ~ServiceManager();
};

#endif