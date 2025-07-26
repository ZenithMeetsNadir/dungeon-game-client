#ifndef SERVICEMANAGER_HPP
#define SERVICEMANAGER_HPP

#include <window/Context.hpp>
#include <client/GameClient.hpp>
#include "DotenvRepository.hpp"

class ServiceManager {
    protected:
        Context *context;

    public:
        GameClient gameClient;
        DotenvRepository dotenvRepo;

        ServiceManager(Context *context);
        ~ServiceManager();
};

#endif