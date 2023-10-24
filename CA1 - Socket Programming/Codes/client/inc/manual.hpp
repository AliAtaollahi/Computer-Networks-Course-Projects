#pragma once

#include <sstream>
#include <map>
#include <vector>

#define SERVER_IP "127.0.0.1"
#define PORT 8181
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

namespace Paths
{
    const std::string CONFIG_SERVER_DATA_PATH = "./../database/config.json";
}

namespace FileDataContainers
{
    struct config
    {
        std::string hostName;
        int commandChannelPort;
    };
}

namespace CommandsNum
{
    enum NotRegisteredCommandsNumbers
    {
        signin = 0,
        signup = 1
    };

    enum ReservationNumbers
    {
        logout = 2,
        View_user_information = 3,
        View_all_users = 4,
        View_rooms_information = 5,
        Booking = 6,
        Canceling = 7,
        pass_day = 8,
        Edit_information = 9,
        Leaving_room = 10,
        Rooms = 11
    };

    namespace RoomCommand
    {
        enum RoomCommandNum
        {
            Rooms_add = -1,
            Rooms_modify = -2,
            Rooms_remove = -3
        };
    }
}

struct commands_names
{
    static std::map<int,std::string> create_map()
    {
        std::map<int,std::string> names = 
        {
            {CommandsNum::signin, "signin"},
            {CommandsNum::signup, "signup"},
            {CommandsNum::logout, "logout"},
            {CommandsNum::View_user_information, "view_user_information"},
            {CommandsNum::View_all_users, "view_all_users"},
            {CommandsNum::View_rooms_information, "view_rooms_information"},
            {CommandsNum::Booking, "book"},
            {CommandsNum::Canceling, "cancel"},
            {CommandsNum::pass_day, "pass_day"},
            {CommandsNum::Edit_information, "edit_information"},
            {CommandsNum::Leaving_room, "leaving_room"},
            {CommandsNum::Rooms, "rooms"},
            {CommandsNum::RoomCommand::Rooms_add, "rooms"},
            {CommandsNum::RoomCommand::Rooms_modify, "rooms"},
            {CommandsNum::RoomCommand::Rooms_remove, "rooms"},
        };
        return names;
    }
    static std::map<int,std::string> desired_map;
};

struct commands_help
{
    static std::map<int,std::string> create_map()
    {
        std::map<int,std::string> messages = 
        {
            {CommandsNum::signin, "Command -> <username> <password>\n\nHelp -> username: Username, password: user password\n"},
            {CommandsNum::signup, "Err -> 311: User Signed up. Enter your username, password, purse, phone and address.\nCommand -> <username> <password> <purse> <phone> <address>\n"},
            {CommandsNum::logout, ""},
            {CommandsNum::View_user_information, ""},
            {CommandsNum::View_all_users, ""},
            {CommandsNum::View_rooms_information, "Command -> filter_empty <empty filter>\n\nHelp -> empty filter: type enable or enter (for admin this is not matter)\n"},
            {CommandsNum::Booking, "Command -> <RoomNum> <NumOfBeds> <CheckInDate> <CheckOutDate>\n\nHelp -> RoomNum: Number of room, NumOfBeds: Number of reservation(per person)\nHelp -> CheckInDate: Start date of reservation, CheckOutDate: End date of reservation\n"},
            {CommandsNum::Canceling, "Command -> <RoomNum> <Num>\n\nHelp -> RoomNum: Number of room, Num: Number of reservation(per person)\n"},
            {CommandsNum::pass_day, "Command -> <value>\n\nHelp -> value: number of day you want to pass\n"},
            {CommandsNum::Edit_information, "Command -> new_password <new password> phone <phone> address <address>\n\nNote: all flags are optional\n"},
            {CommandsNum::Leaving_room, "for user :\nCommand -> value <value>\nfor admin:\nCommand -> value <value> new_capacity <new capacity>\n\nHelp -> value: Room number\nHelp -> new capacity: Enter new capacity of the room\n"},
            {CommandsNum::Rooms, "Command -> add <RoomNum> <Max Capacity> <Price>\nCommand -> modify RoomNum <RoomNum> Max_Capacity <new Max Capacity> Price <new Price>\nCommand -> remove <RoomNum>\n"},
        };
        return messages;
    }
    static std::map<int,std::string> desired_map;
};

namespace MenuMode
{
    enum mode
    {
        not_registered_menu = 1,
        registered_menu = 2,
    };
}

struct commands_list
{
    static std::map<int,std::string> create_map()
    {
        std::map<int,std::string> messages = 
        {
            {MenuMode::not_registered_menu, "0. signin\n1. signup\nCommand -> <Choice number>\n\n"},
            {MenuMode::registered_menu, "3. View_user_information\n4. View_all_users\n5. View_rooms_information\n6. Booking\n7. Canceling\n8. pass_day\n9. Edit_information\n10. Leaving_room\n11. Rooms\n2. logout\n\n"},
        };
        return messages;
    }
    static std::map<int,std::string> desired_map;
};

namespace CommandMode
{
    enum mode
    {
        options_not_required = 1,
        options_required = 2,
    };
}

struct commands_with_options
{
    static std::map<int, std::pair<int, std::vector<std::string>>> create_map()
    {
        std::map<int, std::pair<int, std::vector<std::string>>> options = 
        {
            {CommandsNum::signin, {CommandMode::options_required, {"username", "password"}}},
            {CommandsNum::signup, {CommandMode::options_required, {"username", "password", "purse", "phone", "address"}}},
            {CommandsNum::logout, {CommandMode::options_required, {}}},
            {CommandsNum::View_user_information, {CommandMode::options_required, {}}},
            {CommandsNum::View_all_users, {CommandMode::options_required, {}}},
            {CommandsNum::View_rooms_information, {CommandMode::options_not_required, {"filter_empty"}}},
            {CommandsNum::Booking, {CommandMode::options_required, {"RoomNum", "NumOfBeds", "CheckInDate", "CheckOutDate"}}},
            {CommandsNum::Canceling, {CommandMode::options_required, {"RoomNum", "Num"}}},
            {CommandsNum::pass_day, {CommandMode::options_required, {"value"}}},
            {CommandsNum::Edit_information, {CommandMode::options_not_required, {"new_password", "phone", "address"}}},
            {CommandsNum::Leaving_room, {CommandMode::options_not_required, {"value", "new_capacity"}}},
            {CommandsNum::Rooms, {CommandMode::options_required, {}}},
            {CommandsNum::RoomCommand::Rooms_add, {CommandMode::options_required, {"room_command_request", "RoomNum", "Max_Capacity", "Price"}}},
            {CommandsNum::RoomCommand::Rooms_modify, {CommandMode::options_not_required, {"room_command_request", "RoomNum", "Max_Capacity", "Price"}}},
            {CommandsNum::RoomCommand::Rooms_remove, {CommandMode::options_required, {"room_command_request", "RoomNum"}}},
        };
        return options;
    }
    static std::map<int, std::pair<int, std::vector<std::string>>> desired_map;
};
