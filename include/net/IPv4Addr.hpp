#ifndef IPV4ADDR_HPP
#define IPV4ADDR_HPP

#include "netinclude.h"
#include <string>

struct IPv4Addr {
    in_addr addr;
    u_short port;

    IPv4Addr();
    IPv4Addr(const char *addr, u_short port);

    bool operator==(const IPv4Addr &other) const;

    operator std::string() const;
};

#endif