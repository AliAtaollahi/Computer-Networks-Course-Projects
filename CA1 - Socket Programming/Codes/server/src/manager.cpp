// In the Name of God

#include "manager.hpp"
#include "manual.hpp"
#include "error.hpp"
#include "filehandler.hpp"
#include "Utils.hpp"
#include "person.hpp"
#include "room.hpp"


using namespace std;

Manager::Manager(/* args */)
{
    last_id = 0;
    file_handler = FileHandler();
    load_users_data(Paths::USERS_DATA_PATH);
    load_rooms_data(Paths::ROOMS_DATA_PATH);
}

void Manager::set_today(string server_input)
{
    string date_time = Utils::get_param(server_input, "setTime");
    DateConfig date_config = Utils::convert_to_DataConfig(date_time);
    today = new DateConfig(date_config.day, date_config.month, date_config.year);
    update_all_rooms();
    printf("%stime is set\n %s", KMAG, KWHT);
}

void Manager::add_new_client(int new_client_fd)
{
    printf("%s***\tNew costumer connected.fd=%d\t***%s\n",KCYN, new_client_fd, KWHT);
    send(new_client_fd, WELCOME_MESSAGE, strlen(WELCOME_MESSAGE), 0);
    users_fd_map[new_client_fd] = nullptr;
}

string Manager::handle_command_and_get_response(int fd, string command)
{
    line = command;
    user_fd = fd;
    string command_key = Utils::parse_line(command, ' ');
    try
    {
        auto command_list = get_command_list(user_fd);
        route_command(command_key, command_list);
    } catch (Error& error) {
        save_data();
        return error.what();
    } catch (Message& message) {
        save_data();
        return message.what();
    }
    return "";
}

unordered_map<string, func_ptr> Manager::get_command_list(int fd)
{
    Person* person = users_fd_map[fd];
    return (person == nullptr) ? unregistered_user_list : person->get_command_list();
}

void Manager::route_command(string cmd, const unordered_map<string, func_ptr> &list)
{
    auto func = list.find(cmd);
    if (func == list.end()) throw Error(403);
    (this->*(func->second))();
}

void Manager::load_users_data(string path)
{
    auto users_buffer = file_handler.read_users_info(path);
    for(auto user : users_buffer)
        if(user.admin)
            all_users.push_back(new Admin(user.id, user.user, user.password));
        else
            all_users.push_back(new User(user.id, user.user, user.password, user.purse, user.phoneNumber, user.address));
}

void Manager::load_rooms_data(string path)
{
    auto rooms_buffer = file_handler.read_rooms_info(Paths::ROOMS_DATA_PATH);
    for(auto room : rooms_buffer)
    {
        vector<shared_ptr<Rent>> rents;
        for(auto rent : room.users)
        {
            Person* user;
            for(auto user_ : all_users)
                if(user_->has_this_id(rent.id))
                    user = user_;
            rents.push_back(make_shared<Rent>(user, rent.numOfBeds, 
                                              Utils::convert_to_DataConfig(rent.reserveDate),
                                              Utils::convert_to_DataConfig(rent.checkoutDate)));
        }
        all_rooms.push_back(make_shared<Room>(room.number, room.status, room.price, 
                                              room.maxCapacity, room.capacity, rents));
    }
}

void Manager::save_data()
{
    save_users_data(Paths::USERS_DATA_PATH);
    save_rooms_data(Paths::ROOMS_DATA_PATH);
}
void Manager::save_users_data(std::string path)
{
    vector<FileDataContainers::UserInfo> info;
    for(auto user : all_users)
        info.push_back(user->get_data_for_write());
    file_handler.write_users_info(path, info);
}
void Manager::save_rooms_data(std::string path)
{
    vector<FileDataContainers::RoomInfo> info;
    for(auto room : all_rooms)
        info.push_back(room->get_data_for_write());
    file_handler.write_rooms_info(path, info);
}