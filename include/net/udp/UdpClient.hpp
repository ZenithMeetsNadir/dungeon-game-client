#ifndef UDPCLIENT_HPP
#define UDPCLIENT_HPP

#include "UdpServer.hpp"

class UdpClient : public UdpServer {
    protected:
        std::string getServiceName() const override;
        
        void listenLoop() const override;

    public:
        UdpClient(IPv4Addr addr);
        ~UdpClient();

        bool open() override;
        int sendMsg(const char *data, size_t size) const;
};

#endif