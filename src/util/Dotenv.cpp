#include <util/Dotenv.hpp>

const char *const Dotenv::envPath = "assets/.env";

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
                    throw DotenvKeyNotFoundException();

                return line.substr(equalPos + 1);
            }
        }
    }
    
    throw DotenvException();
}

void Dotenv::set(const char *key, const char *value) {
    if (fileStream.is_open()) {
        fileStream.clear();
        fileStream.seekg(0, std::ios::beg);
        size_t length = 0;
        for (; fileStream.get() != EOF; ++length) { }

        fileStream.clear();
        fileStream.seekg(0, std::ios::beg);
        char *buffer = new char[length + 1];

        int ch;
        size_t i = 0;
        while (i < length)
            buffer[i++] = static_cast<char>(fileStream.get());

        buffer[i] = '\0';

        std::string fileContent(buffer);
        delete[] buffer;

        std::cout << fileContent << std::endl;

        fileStream.clear();
        size_t pos = fileContent.find(key);
        if (pos == std::string::npos) {
            fileStream.seekp(0, std::ios::end);
            fileStream << '\n' << key << "=" << value << std::endl;
        } else {
            size_t startPos = pos + strlen(key) + 1;
            size_t endPos = fileContent.find('\n', startPos);
            fileContent.replace(startPos, endPos - startPos, value);

            std::cout << fileContent << std::endl;

            fileStream.seekp(0, std::ios::beg);
            fileStream << fileContent << std::endl;
        }
    }
}