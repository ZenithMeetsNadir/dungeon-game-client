#ifndef NETUTIL_HPP
#define NETUTIL_HPP

#include "netinclude.h"
#include <iostream>

bool initWsa();
void deinitWsa();

bool setNonBlocking(SOCKET sock);
bool connected(SOCKET sock);
bool checkRecvResult(int &recvRes, int bufferSize);

#endif