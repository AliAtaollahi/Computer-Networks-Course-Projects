#pragma once

#include <vector>
#include <cstring>
#include <string>
#include <sstream>
#include <map>

#include "class_names.hpp"

#define MAX_CONNECTIONS 20
#define MAX_NAME_SIZE 256
#define MAX_STRING_SIZE 4096
#define NOT_CONNECTED -3
#define NOT_FOUND -1
#define STD_IN 0
#define STD_OUT 1

//colors
#define KNRM "\x1B[0m"
#define KRED "\x1B[31m"
#define KGRN "\x1B[32m"
#define KYEL "\x1B[33m"
#define KBLU "\x1B[34m"
#define KMAG "\x1B[35m"
#define KCYN "\x1B[36m"
#define KWHT "\x1B[37m"

//messages
#define WELCOME_MESSAGE "welcome to khane ghamar khanom\n0. signin\n1. signup\nCommand -> <Choice number>\n"

namespace Paths
{
    const std::string CONFIG_SERVER_DATA_PATH = "./../database/config.json";
    const std::string USERS_DATA_PATH = "./../database/users.json";
    const std::string ROOMS_DATA_PATH = "./../database/rooms.json";
    const std::string LOG_CLUSTER_PATH = "./../cluster.log";
}

namespace FileDataContainers
{
    struct config
    {
        std::string hostName;
        int commandChannelPort;
    };

    struct UserInfo
    {
        int id, purse;
        std::string user, password, phoneNumber, address;
        bool admin;
    };

    struct RentInfo
    {
        int id, numOfBeds;
        std::string reserveDate, checkoutDate;
    };
    
    struct RoomInfo
    {
        int number, status, price, maxCapacity, capacity;
        std::vector<RentInfo> users;
    };
}

struct Info
{
    static std::map<int,std::string> create_map()
    {
        std::map<int,std::string> messages = 
        {
            {101, "DESIRED_ROOM_WAS_NOT_FOUND"},
            {102, "YOUR_RESERVATION_WAS_NOT_FOUND"},
            {103, "YOU_MUST_LEAVE_NOT_CANCEL"},
            {104, "SUCCESSFULLY_ADDED"},
            {105, "SUCCESSFULLY_MODIFIED"},
            {106, "SUCCESSFULLY_DELETED"},
            {108, "YOUR_ACCOUNT_BALANCE_IS_NOT_ENOUGH"},
            {109, "THE_ROOM_CAPACITY_IS_FULL"},
            {110, "SUCCESSFULLY_DONE"},
            {111, "THIS_ROOM_ALREADY_EXISTS"},
            {112, "YOU_ARE_NOT_IN_ROOM_YET"},
            {201, "USER_LOGED_OUT_SUCCESSFULLY"},
            {230, "USER_LOGED_IN"},
            {231, "USER_SUCCESSFULLY_SIGNED_UP"},
            {232, "YOU_LOGED_IN_BEFORE"},
            {311, "USER_SIGNED_UP.ENTER_YOUR -PASS-MONEY-PHONE-ADDR-"},
            {312, "INFORMATION_CHANGED_SUCCESSFULLY"},
            {401, "INVALID_VALUE"},
            {403, "ACCESS_DENIED"},
            {412, "INVALID_CAPACITY_VALUE"},
            {413, "SUCCESSFULLY_LEAVING"},
            {430, "INVALID_USERNAME/PASSWORD"},
            {451, "USER_ALREADY_EXISTS"},
            {503, "BAD_SEQUENCE_OF_COMMANDS"}
        };
        return messages;
    }
    static std::map<int,std::string> status;
};

namespace RoomStatus 
{
    const int EMPTY = 0;
    const int FULL = 1;
    const std::vector<std::string> str_form = {
        "EMPTY",
        "FULL"
    };
}

namespace EmptyFilterStatus 
{
    const int ENABLE = 1;
    const int DISABLE = 0;
}
