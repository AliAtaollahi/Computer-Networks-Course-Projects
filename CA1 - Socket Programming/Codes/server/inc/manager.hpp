#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <memory>

#include "class_names.hpp"
#include "filehandler.hpp"

class Manager
{
public:
    Manager(/* args */);

    void set_today(std::string server_input);
    void add_new_client(int new_client_fd);
    std::string handle_command_and_get_response(int fd, std::string command);
    std::unordered_map<std::string, func_ptr> get_command_list(int fd);
    void route_command(std::string cmd, const std::unordered_map<std::string, func_ptr> &list);
    void signup();
    void signin();
    void view_user_information();
    void view_all_users();
    void view_rooms_information_for_user();
    void view_rooms_information_for_admin();
    void book();
    void show_reservations();
    void cancel();
    void pass_day();
    void edit_information_for_user();
    void edit_information_for_admin();
    void leaving_room_for_user();
    void leaving_room_for_admin();
    void rooms();
    void logout();
    void check_exist_username(std::string username);
    void find_signed_in_user(std::string username, std::string password);
    std::string get_rooms_info(bool is_admin, int filter_empty_status);
    void book_with_options(int RoomNum,int NumOfBeds, std::string CheckInDate, std::string CheckOutDate);
    void update_all_rooms();
    void rooms_add();
    void rooms_modify();
    void rooms_remove();
    void check_room_is_not_exist(std::string RoomNum);
    int  find_room(int room_num);
    void check_room_is_empty(int room_index);
    void save_data();
private:
    void load_users_data(std::string path);
    void load_rooms_data(std::string path);
    void save_users_data(std::string path);
    void save_rooms_data(std::string path);
    FileHandler file_handler;
    std::vector<Person*> all_users;
    std::vector<std::shared_ptr<Room>> all_rooms;
    std::unordered_map<int, Person*> users_fd_map;
    std::string line;
    int user_fd;
    int last_id;
    DateConfig* today;
};

const std::unordered_map<std::string, func_ptr> unregistered_user_list = {
    {"signup", &Manager::signup},
    {"signin", &Manager::signin},
};

const std::unordered_map<std::string, func_ptr> user_command_list = {
    {"view_user_information", &Manager::view_user_information},
    {"view_rooms_information", &Manager::view_rooms_information_for_user},
    {"book", &Manager::book},
    {"show_reservations", &Manager::show_reservations},
    {"cancel", &Manager::cancel},
    {"edit_information", &Manager::edit_information_for_user},
    {"leaving_room", &Manager::leaving_room_for_user},
    {"logout", &Manager::logout},
};

const std::unordered_map<std::string, func_ptr> admin_command_list = {
    {"view_user_information", &Manager::view_user_information},
    {"view_all_users", &Manager::view_all_users},
    {"view_rooms_information", &Manager::view_rooms_information_for_admin},
    {"pass_day", &Manager::pass_day},
    {"edit_information", &Manager::edit_information_for_admin},
    {"leaving_room", &Manager::leaving_room_for_admin},
    {"rooms", &Manager::rooms},
    {"logout", &Manager::logout},
};

const std::unordered_map<std::string, func_ptr> rooms_command_list = {
    {"add", &Manager::rooms_add},
    {"modify", &Manager::rooms_modify},
    {"remove", &Manager::rooms_remove},
};