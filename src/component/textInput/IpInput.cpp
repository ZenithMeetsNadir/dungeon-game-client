#include <component/textInput/IpInput.hpp>

IpInput::IpInput(Context *context)
    : TextInput(context)
{ }

IpInput::~IpInput() { }

void IpInput::determineColor() {
    TextInput::determineColor();

    if (!isValid() && !text.empty())
        backColor = invalidBackColor;
}

bool IpInput::isValidIpChar(char c) {
    return (c >= '0' && c <= '9') || c == '.' || c == ':';
}

bool IpInput::isValid() const {
    if (text.empty())
        return false;

    size_t startPos = 0;
    size_t dotPos = 0;
    bool hasPort = false;

    for (int dots = 0; dots < 4; dots++) {
        dotPos = text.find('.', dotPos + 1);
        if (dotPos == std::string::npos) {
            dotPos = text.find(':');
            if (dotPos == std::string::npos)
                dotPos = text.length();
            else 
                hasPort = true;
        }

        if (startPos >= dotPos)
            return false;

        auto digits = text.substr(startPos, dotPos - startPos);
        std::cout << "startPos: " << startPos << "; dotPos: " << dotPos << std::endl << digits << ": " << (digits.front() == '0' && digits.length() > 1) << std::endl;
        if (std::stoi(digits) > 255 || digits.front() == '0' && digits.length() > 1)
            return false;

        startPos = dotPos + 1;
    }

    if (!hasPort)
        return true;
    
    auto port = text.substr(dotPos + 1);
    if (port.empty() || std::stoi(port) > 65535 || port.front() == '0' && port.length() > 1)
        return false;

    return true;
}

bool IpInput::handleEvents(const SDL_Event &event) {
    bool dirty = TextInput::handleEvents(event);

    while (!text.empty() && !isValidIpChar(text.back())) {
        text.pop_back();
        dirty |= true;
    }

    determineColor();

    return dirty;
}