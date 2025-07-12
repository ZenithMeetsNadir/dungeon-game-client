#ifndef NETINCLUDE_H
#define NETINCLUDE_H

#ifdef _WIN32
    #include <WinSock2.h>

    #define WOULDBLOCK WSAEWOULDBLOCK
    #define MSGTOOBIG WSAEMSGSIZE
    #define CONNECTWOULDBLOCK WSAEWOULDBLOCK

    #define GETLASTERROR() WSAGetLastError()

    typedef int socklen_t;
#endif
#ifdef __linux__
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <fcntl.h>

    #define INVALID_SOCKET (-1)
    #define SOCKET_ERROR (-1)
    #defind WOULDBLOCK EWOULDBLOCK
    #define MSGTOOBIG 0
    #define CONNECTWOULDBLOCK EINPROGRESS

    #define GETLASTERROR() errno

    inline int closesocket(int sockfd) { return close(sockfd); }

    typedef int SOCKET;
#endif

#endif