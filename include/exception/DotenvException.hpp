#pragma once
#include <exception>

class DotenvException : public std::exception { };
class DotenvKeyNotFoundException : public std::exception { };