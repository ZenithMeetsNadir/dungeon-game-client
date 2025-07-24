#include <component/textInput/IpInput.hpp>

IpInput::IpInput(Context *context)
    : TextInput(context)
{ 
    onClickCallback = std::function<void()>();
}

IpInput::~IpInput() { }

void IpInput::determineColor() {
    TextInput::determineColor();

    if (!isValid() && !text.empty())
        backColor = invalidBackColor;
}

void IpInput::purifyInput() {
    for (size_t i = 0; i < text.length(); ++i) {
        if (!isValidIpChar(text[i]))
            text.erase(text.begin() + i++);
    }
}

bool IpInput::containsInvalidIpChar() const {
    for (char c : text) {
        if (!isValidIpChar(c))
            return true;
    }

    return false;
}

bool IpInput::isValidIpChar(char c) {
    return (c >= '0' && c <= '9') || c == '.' || c == ':';
}

bool IpInput::isValidExplicitCheck() const {
    if (text.empty())
        return false;

    std::cout << text << std::endl;

    size_t startPos = 0;
    size_t dotPos = 0;
    bool hasPort = false;
    int dots = 0;

    try {
        for (; dots < 4; ++dots) {
            dotPos = text.find('.', dotPos);
            if (dotPos == std::string::npos) {
                dotPos = text.find(':');
                if (dotPos == std::string::npos)
                    dotPos = text.length();
                else 
                    hasPort = true;
            } else if (dots == 3)
                return false;

            if (startPos >= dotPos)
                return false;

            auto digits = text.substr(startPos, dotPos - startPos);
            if (std::stoi(digits) > 255 || digits.front() == '0' && digits.length() > 1)
                return false;

            startPos = ++dotPos;
        }

        if (!hasPort)
            return true;
        
        auto port = text.substr(dotPos);
        if (port.empty() || std::stoi(port) > 65535 || port.front() == '0' && port.length() > 1)
            return false;
    } catch (std::exception) {
        return false;
    }

    return true;
}

std::string IpInput::getAddr() const {
    if (!isValid())
        throw IPv4AddrException();

    size_t count = hasPort() ? text.find(':') : text.length();
    return text.substr(0, count);
}

u_short IpInput::getPort() const {
    if (!hasPort())
        return static_cast<u_short>(std::stoi(Dotenv::dotenv.get("lanport")));

    if (isValid()) {
        size_t sepPos = text.find(':');
        size_t count = text.length() - sepPos;
        return static_cast<u_short>(std::stoi(text.substr(sepPos + 1, count)));
    }

    throw IPv4AddrException();
}

IPv4Addr IpInput::getQualfAddr() const {
    return IPv4Addr(getAddr().c_str(), getPort());
}

bool IpInput::handleEvents(const SDL_Event &event) {
    bool dirty = TextInput::handleEvents(event);

    while (!text.empty() && !isValidIpChar(text.back())) {
        text.pop_back();
        dirty |= true;
    }

    return dirty;
}