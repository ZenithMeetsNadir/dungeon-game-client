#ifndef DOTENV_HPP
#define DOTENV_HPP

#include <string>
#include <iostream>
#include <fstream>

class Dotenv {
    private: 
        std::ifstream fileStream;

    public:
        static const char *const envPath;
        static Dotenv dotenv;

        Dotenv(const char *filePath = envPath);
        ~Dotenv();

        std::string get(const char *key);
};

#endif