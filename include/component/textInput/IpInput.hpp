#ifndef IPINPUT_HPP
#define IPINPUT_HPP

#include "TextInput.hpp"
#include <util/Dotenv.hpp>
#include <net/IPv4Addr.hpp>
#include <exception/IPv4AddrException.hpp>
#include <functional>

class IpInput : public TextInput {
    protected:
        void determineColor() override;

        void purifyInput();
        bool containsInvalidIpChar() const;
        bool isValidExplicitCheck() const override;

        static bool isValidIpChar(char c);

    public:
        IpInput(Context *context);
        ~IpInput();

        bool hasPort() const;

        std::string getAddr() const;
        u_short getPort() const;
        IPv4Addr getQualfAddr() const;

        bool handleEvents(const SDL_Event &event) override;
};

inline bool IpInput::hasPort() const {
    size_t pos = text.find(':');
    return pos != std::string::npos && pos < text.length() - 1;
}

#endif