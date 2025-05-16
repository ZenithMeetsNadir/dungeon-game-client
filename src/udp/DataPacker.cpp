#include <udp/DataPacker.hpp>

DataPacker::DataPacker(const char *pw, const char *prefix, char delim)
    : pw(pw), prefix(prefix), delim(delim)
{ }

void DataPacker::keyValueMode(char keyValueDelim) {
    this->keyValue = true;
    this->keyValueDelim = keyValueDelim;
}

std::string DataPacker::whichevercrypt(std::string data) {
    std::string str;
    size_t pwLen = strlen(pw);

    for (size_t i = 0; i < data.length(); ++i) {
        str += data[i] ^ pw[i % pwLen];
    }

    return str;
}

std::string DataPacker::valueOf(std::string data, const char *key) const {
    size_t sIndex = data.find(key);
    size_t eIndex = data.find(delim, sIndex);

    if (sIndex == std::string::npos)
        return "";

    size_t valueStart = sIndex + strlen(key) + 1;
    return data.substr(valueStart, eIndex - valueStart);
}

std::string DataPacker::message() const {
    return prefix;
}

void DataPacker::msgAppend(std::string &message, const char *key, std::string value) const {
    message += delim;

    if (key) {
        message += key;
        message += keyValueDelim;
    }

    message += value;
}
