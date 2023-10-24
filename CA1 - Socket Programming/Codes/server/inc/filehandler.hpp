#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>

#if __has_include(<jsoncpp/json/json.h>)
# include <jsoncpp/json/json.h>
#elif __has_include(<json/json.h>)
#include <json/json.h>
#endif

#include "manual.hpp"
#include "person.hpp"

class FileHandler
{
public:
    FileHandler();
    FileDataContainers::config read_config(std::string path);
    std::vector<FileDataContainers::UserInfo> read_users_info(std::string path);
    std::vector<FileDataContainers::RoomInfo> read_rooms_info(std::string path);
    std::vector<FileDataContainers::RentInfo> get_rents(Json::Value users);
    Person* make_user(FileDataContainers::UserInfo user);
    void write_users_info(std::string path, std::vector<FileDataContainers::UserInfo> info);
    void write_rooms_info(std::string path, std::vector<FileDataContainers::RoomInfo> info);
    Json::Value get_rent_info(FileDataContainers::RentInfo traveler);
};
