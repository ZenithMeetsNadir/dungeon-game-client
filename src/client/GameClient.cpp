#include <client/GameClient.hpp>
#include <window/Context.hpp>

GameClient::GameClient(Context *context) {
    this->context = context;
    serverPw = context->service->dotenvRepo.getDefaultPassword();
    serverVfytkn = context->service->dotenvRepo.getDefaultVerifyToken();

    dp = new DataPacker(serverPw.c_str(), serverVfytkn.c_str());
    dp->keyValueMode();
}

GameClient::~GameClient() {
    try {
        disconnectBlocking();
    } catch (const NetworkInitException &e) { }

    delete dp;
}

bool GameClient::open(const IPv4Addr &serverAddr) {
    udpClient = new UdpClient(serverAddr);
    if (!udpClient->open()) {
        delete udpClient;
        udpClient = nullptr;
        return false;
    }
     
    udpClient->setDispatchFunc(EncapsulateDispatchFunc{ this });

    tcpClient = new TcpClient(serverAddr);
    if (!tcpClient->open()) {
        delete tcpClient;
        tcpClient = nullptr;

        udpClient->close();
        delete udpClient;
        udpClient = nullptr;

        return false;
    }

    tcpClient->setDispatchFunc(EncapsulateDispatchFunc{ this });

    return true;
}

void GameClient::waitConnectionBlocking() {
    if (!udpClient || !tcpClient) {
        std::cerr << "GameClient not opened properly (call open)" << std::endl;
        throw NetworkInitException();
    }

    // simulate connection process for debug purposes
    std::this_thread::sleep_for(std::chrono::milliseconds(3000));

    for (int i = 0; i <= 10; ++i) {
        if (tcpClient) {
            if (tcpClient->isReady())
                break;
        } else 
            throw TcpConnectAbortedException();

        if (i == 10) {
            std::cerr << "tcp client is not ready after 10 seconds, giving up" << std::endl;
            throw NetworkInitException();
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    }

    udpClient->listen();
    tcpClient->listen();
}

void GameClient::notifyDisconnectBlocking() {
    if (!tcpClient)
        return;

    tcpClient->stopListening();

    std::string disconnectMsg = dp->message();
    dp->msgAppend(disconnectMsg, "die", "");
    std::string encoded = dp->whichevercrypt(disconnectMsg);

    tcpClient->sendMsg(encoded.c_str(), encoded.size());
}

void GameClient::close() {
    if (udpClient) {
        udpClient->close();
        delete udpClient;
        udpClient = nullptr;
    }

    if (tcpClient) {
        tcpClient->close();
        delete tcpClient;
        tcpClient = nullptr;
    }
}

void GameClient::openConnect(const IPv4Addr &serverAddr) {
    if (!open(serverAddr)) {
        std::cerr << "failed to open GameClient" << std::endl;
        throw NetworkInitException();
    }
}

void GameClient::connectBlocking(const IPv4Addr &serverAddr) {
    openConnect(serverAddr);
    waitConnectionBlocking();
}

void GameClient::disconnectBlocking() {
    notifyDisconnectBlocking();
    close();
}

void GameClient::sendUdpMsg(const char *data) {
    if (udpClient)
        udpClient->sendMsg(data, strlen(data));
}

void GameClient::sendTcpMsg(const char *data) {
    if (tcpClient)
        tcpClient->sendMsg(data, strlen(data));
}

void GameClient::dispatchUdpMsg(const IPv4Addr &addr, const char *data, size_t size) {
    for (auto handler : dispatchUdpHandlers) {
        handler->operator()(addr, data, size);
    }
}

void GameClient::dispatchTcpMsg(const IPv4Addr &addr, const char *data, size_t size) {
    for (auto handler : dispatchTcpHandlers) {
        handler->operator()(addr, data, size);
    }
}

void GameClient::addDispatchUdpHandler(DispatchHandlerT *handler) {
    dispatchUdpHandlers.push_back(handler);
}

void GameClient::addDispatchTcpHandler(DispatchHandlerT *handler) {
    dispatchTcpHandlers.push_back(handler);
}

void GameClient::removeDispatchUdpHandler(DispatchHandlerT *rmHandler) {
    size_t index = 0;
    for (auto handler : dispatchUdpHandlers) {
        if (handler == rmHandler) {
            dispatchUdpHandlers.erase(dispatchUdpHandlers.begin() + index);
            break;
        }

        ++index;
    }
}

void GameClient::removeDispatchTcpHandler(DispatchHandlerT *rmHandler) {
    size_t index = 0;
    for (auto handler : dispatchTcpHandlers) {
        if (handler == rmHandler) {
            dispatchTcpHandlers.erase(dispatchTcpHandlers.begin() + index);
            break;
        }

        ++index;
    }
}