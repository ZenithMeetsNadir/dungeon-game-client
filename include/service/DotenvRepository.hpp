#ifndef DOTENVREPOSITORY_HPP
#define DOTENVREPOSITORY_HPP

#include <util/Dotenv.hpp>
#include <net/IPv4Addr.hpp>
#include <exception/DotenvException.hpp>
#include <string>

class DotenvRepository {
    protected:
        Dotenv dotenv;

    public:
        DotenvRepository();
        ~DotenvRepository();

        std::string getDefaultPassword();
        std::string getDefaultVerifyToken();
        std::string getLanPort();
        std::string getPlayerNameAtLanServer(const std::string &serverName);
        std::string getPlayerNameAtServer(const IPv4Addr &serverAddr);

        void updatePlayerNameAtLanServer(const std::string &serverName, const char *playerName);
        void updatePlayerNameAtServer(const IPv4Addr &serverAddr, const char *playerName);
};

inline std::string DotenvRepository::getDefaultPassword() {
    return dotenv.get("pw");
}

inline std::string DotenvRepository::getDefaultVerifyToken() {
    return dotenv.get("vfytkn");
}

inline std::string DotenvRepository::getLanPort() {
    return dotenv.get("lanport");
}

inline std::string DotenvRepository::getPlayerNameAtLanServer(const std::string &serverName) {
    return dotenv.get(("plrname_at_\"" + serverName + "\"").c_str());
}

inline std::string DotenvRepository::getPlayerNameAtServer(const IPv4Addr &serverAddr) {
    return dotenv.get(("plrname_at_\"" + static_cast<std::string>(serverAddr) + "\"").c_str());
}

inline void DotenvRepository::updatePlayerNameAtLanServer(const std::string &serverName, const char *playerName) {
    dotenv.set(("plrname_at_\"" + serverName + "\"").c_str(), playerName);
}

inline void DotenvRepository::updatePlayerNameAtServer(const IPv4Addr &serverAddr, const char *playerName) {
    dotenv.set(("plrname_at_\"" + static_cast<std::string>(serverAddr) + "\"").c_str(), playerName);
}

#endif
