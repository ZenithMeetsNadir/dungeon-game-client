#ifndef DATAPACKER_HPP
#define DATAPACKER_HPP

#include <string.h>
#include <iostream>
#include <string>
#include <exception/DataPackerKeyNotFoundException.hpp>

struct DataPacker {
    const char *pw;
    const char *prefix;
    char delim = '&';
    bool keyValue = false;
    char keyValueDelim = '=';

    DataPacker(const char *pw, const char *prefix, char delim = '&');

    void keyValueMode(char keyValueDelim = '=');

    /// DISCLAIMER:
    /// the following encryption and decryption is merely a xor obfuscation and guarantees less than no security
    std::string whichevercrypt(std::string data);

    bool verify(std::string data) {
        return data.find(prefix) == 0;
    }

    std::string valueOf(std::string data, const char *key) const;
    std::string message() const;
    void msgAppend(std::string &message, const char *key, const std::string &value) const;
};

#endif
