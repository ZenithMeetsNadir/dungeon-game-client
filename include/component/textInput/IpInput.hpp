#ifndef IPINPUT_HPP
#define IPINPUT_HPP

#include "TextInput.hpp"

class IpInput : public TextInput {
    protected:
        void determineColor() override;
        
        static bool isValidIpChar(char c);

    public:
        IpInput(Context *context);
        ~IpInput();

        bool hasPort() const;
        bool isValid() const;

        bool handleEvents(const SDL_Event &event) override;
};

inline bool IpInput::hasPort() const {
    return text.find(':') != std::string::npos;
}

#endif