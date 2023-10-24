// In the Name of God

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
#include "filehandler.hpp"

using namespace std;

FileHandler::FileHandler()
{
}

FileDataContainers::config FileHandler::read_config(string path)
{
    FileDataContainers::config config;
    Json::Value root;
    std::ifstream input_file(path, std::ifstream::binary);
    input_file >> root;

    for(auto line = root.begin(); line != root.end() ; line++)
    {
        config.hostName = (*line)["hostName"].asString();
        config.commandChannelPort = (*line)["commandChannelPort"].asInt();
    }
    return config;
}