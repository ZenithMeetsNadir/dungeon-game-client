#include <window/ServiceManager.hpp>

ServiceManager::ServiceManager() {
    gameClient = new GameClient();
}

ServiceManager::~ServiceManager() {
    delete gameClient;
}