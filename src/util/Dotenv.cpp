#include <util/Dotenv.hpp>

const char *const Dotenv::envPath = "assets/.env";
Dotenv Dotenv::dotenv = Dotenv();

Dotenv::Dotenv(const char *filePath) {
    fileStream = std::fstream(filePath, std::fstream::in | std::fstream::out);
    if (!fileStream.is_open()) {
        std::cerr << "failed to open .env file: " << filePath << std::endl;
        throw 1;
    }
}

Dotenv::~Dotenv() {
    if (fileStream.is_open())
        fileStream.close();
}

std::string Dotenv::get(const char *key) {
    if (fileStream.is_open()) {
        fileStream.clear();
        fileStream.seekg(0, std::ios::beg);
        std::string line;
        while (std::getline(fileStream, line)) {
            if (line.find(key) == 0) {
                size_t equalPos = line.find('=');
                if (equalPos == std::string::npos) 
                    throw DotenvException();

                return line.substr(equalPos + 1);
            }
        }
    }
    
    throw DotenvException();
}

void Dotenv::set(const char *key, const char *value) {
    if (fileStream.is_open()) {
        fileStream.clear();
        fileStream.seekp(0, std::ios::end);
        fileStream << std::endl << key << "=" << value;
        fileStream.flush();
    }
}