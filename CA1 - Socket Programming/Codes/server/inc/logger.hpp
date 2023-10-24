#pragma once

#include <string>

class Logger
{
public:
    Logger();
    void log(std::string roll, int fd, std::string command);
};
