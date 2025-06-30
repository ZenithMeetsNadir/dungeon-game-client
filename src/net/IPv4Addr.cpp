#include <net/IPv4Addr.hpp>

IPv4Addr::IPv4Addr() {
    this->addr.s_addr = INADDR_ANY;
    this->port = htons(0);
}

IPv4Addr::IPv4Addr(const char *addr, u_short port) {
    this->addr.s_addr = inet_addr(addr);
    this->port = htons(port);
}

bool IPv4Addr::operator==(const IPv4Addr &other) const {
    return addr.s_addr == other.addr.s_addr && port == other.port;
}

IPv4Addr::operator std::string() const {
    return std::string(inet_ntoa(this->addr)) + ":" + std::to_string(ntohs(this->port));
}