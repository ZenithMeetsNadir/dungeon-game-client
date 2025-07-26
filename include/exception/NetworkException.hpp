#pragma once
#include <exception>

class NetworkInitException : public std::exception { };
class TcpConnectAbortedException : public std::exception { };