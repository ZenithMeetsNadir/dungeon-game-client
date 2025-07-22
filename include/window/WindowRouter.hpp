#ifndef WINDOWROUTER_HPP
#define WINDOWROUTER_HPP

#include "Context.hpp"
#include <sdls.h>
#include <iostream>

class WindowRouter {
    protected:
        Context *context;

        /// @brief used to signal that the underlying switch action violates the flow direction and will not take place
        void flowDirectionViolation(const char *action);

    public:
        WindowRouter(Context *context);
        ~WindowRouter();

        void gameConnect();
        void gameDisconnect();
        void gameLoad();
        void gameExit();
};

#endif