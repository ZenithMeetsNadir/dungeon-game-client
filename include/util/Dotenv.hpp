#ifndef DOTENV_HPP
#define DOTENV_HPP

#include <string.h>
#include <string>
#include <iostream>
#include <fstream>
#include <exception/DotenvException.hpp>

#define READ_FILE_BUFFER_CHUNK 128

class Dotenv {
    private: 
        std::fstream fileStream;

    public:
        static const char *const envPath;

        Dotenv(const char *filePath = envPath);
        ~Dotenv();

        std::string get(const char *key);
        void set(const char *key, const char *value);
};

#endif