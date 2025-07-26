#include <service/ServiceManager.hpp>

ServiceManager::ServiceManager(Context *context) 
    : context(context), gameClient(context), dotenvRepo()
{ }

ServiceManager::~ServiceManager() { }