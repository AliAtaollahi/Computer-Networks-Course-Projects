// In the Name of God

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
#include "filehandler.hpp"
#include "person.hpp"
#include "Utils.hpp"

using namespace std;

FileHandler::FileHandler()
{
}

FileDataContainers::config FileHandler::read_config(string path)
{
    FileDataContainers::config config;
    Json::Value root;
    ifstream input_file(path, ifstream::binary);
    input_file >> root;

    for(auto line = root.begin(); line != root.end() ; line++)
    {
        config.hostName = (*line)["hostName"].asString();
        config.commandChannelPort = (*line)["commandChannelPort"].asInt();
    }
    input_file.close();
    return config;
}

vector<FileDataContainers::UserInfo> FileHandler::read_users_info(string path)
{
    Json::Value root;
    ifstream input_file(path, ifstream::binary);
    vector<FileDataContainers::UserInfo> info;
    input_file >> root;

    for(auto user = root.begin(); user != root.end() ; user++)
    {
        FileDataContainers::UserInfo user_info;

        user_info.id = (*user)["id"].asInt();
        user_info.user = (*user)["user"].asString();
        user_info.password = (*user)["password"].asString();
        user_info.admin = (*user)["admin"].asBool();
        if(!user_info.admin)
        {
            user_info.purse = (*user)["purse"].asInt();
            user_info.phoneNumber = (*user)["phoneNumber"].asString();
            user_info.address = (*user)["address"].asString();
        }
        info.push_back(user_info);
    }
    return info;
}

vector<FileDataContainers::RoomInfo> FileHandler::read_rooms_info(string path)
{
    Json::Value root;
    ifstream input_file(path, ifstream::binary);
    vector<FileDataContainers::RoomInfo> info;
    input_file >> root;

    for(auto user = root.begin(); user != root.end() ; user++)
    {
        FileDataContainers::RoomInfo room_info;

        room_info.number = (*user)["number"].asInt();
        room_info.status = (*user)["status"].asInt();
        room_info.price = (*user)["price"].asInt();
        room_info.maxCapacity = (*user)["maxCapacity"].asInt();
        room_info.capacity = (*user)["capacity"].asInt();
        room_info.users = get_rents((*user)["users"]);
        info.push_back(room_info);
    }
    return info;
}

vector<FileDataContainers::RentInfo> FileHandler::get_rents(Json::Value users)
{
    vector<FileDataContainers::RentInfo> info;
    for(auto rent = users.begin(); rent != users.end() ; rent++)
    {
        FileDataContainers::RentInfo rent_info;

        rent_info.id = (*rent)["id"].asInt();
        rent_info.numOfBeds = (*rent)["numOfBeds"].asInt();
        rent_info.reserveDate = (*rent)["reserveDate"].asString();
        rent_info.checkoutDate = (*rent)["checkoutDate"].asString();
        info.push_back(rent_info);
    }
    return info;
}


void FileHandler::write_users_info(string path, vector<FileDataContainers::UserInfo> info)
{
    Json::Value root;
    ofstream output_file(path, ifstream::binary);
    for(auto user_info : info)
    {
        Json::Value user; 

        user["id"] = user_info.id;
        user["user"] = user_info.user;
        user["password"] = user_info.password;
        user["admin"] = user_info.admin;
        if(!user_info.admin)
        {
            user["purse"] = user_info.purse;
            user["phoneNumber"] = user_info.phoneNumber;
            user["address"] = user_info.address;
        }

        root.append(user);
    }
    output_file << root << endl;
}

void FileHandler::write_rooms_info(string path, vector<FileDataContainers::RoomInfo> info)
{
    Json::Value root;
    ofstream output_file(path, ifstream::binary);
    for(auto room_info : info)
    {
        Json::Value room; 

        room["number"] = room_info.number;
        room["status"] = room_info.status;
        room["price"] = room_info.price;
        room["maxCapacity"] = room_info.maxCapacity;
        room["capacity"] = room_info.capacity;
        for(auto rent : room_info.users)
            room["users"].append(get_rent_info(rent));

        root.append(room);
    }
    output_file << root << endl;
}

Json::Value FileHandler::get_rent_info(FileDataContainers::RentInfo traveler)
{
    Json::Value root;
    root["id"] = traveler.id;
    root["numOfBeds"] = traveler.numOfBeds;
    root["reserveDate"] = traveler.reserveDate;
    root["checkoutDate"] = traveler.checkoutDate;
    return root;  
}