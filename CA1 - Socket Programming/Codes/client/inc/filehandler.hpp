#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#if __has_include(<jsoncpp/json/json.h>)
# include <jsoncpp/json/json.h>
#elif __has_include(<json/json.h>)
#include <json/json.h>
#endif

#include "manual.hpp"

class FileHandler
{
public:
    FileHandler();
    FileDataContainers::config read_config(std::string path);
};
