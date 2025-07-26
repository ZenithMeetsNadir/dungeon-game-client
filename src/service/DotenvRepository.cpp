#include <service/DotenvRepository.hpp>

DotenvRepository::DotenvRepository()
    : dotenv(Dotenv::envPath)
{ }

DotenvRepository::~DotenvRepository() { }