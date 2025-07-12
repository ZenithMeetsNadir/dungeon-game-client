#ifndef DOTENV_HPP
#define DOTENV_HPP

#include <string>
#include <iostream>
#include <fstream>
#include <exception/DotenvException.hpp>

class Dotenv {
    private: 
        std::fstream fileStream;

    public:
        static const char *const envPath;
        static Dotenv dotenv;

        Dotenv(const char *filePath = envPath);
        ~Dotenv();

        std::string get(const char *key);
        void set(const char *key, const char *value);
};

#endif