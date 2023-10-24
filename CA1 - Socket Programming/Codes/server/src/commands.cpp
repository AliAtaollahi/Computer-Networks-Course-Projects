#include "manager.hpp"
#include "manual.hpp"
#include "error.hpp"
#include "filehandler.hpp"
#include "Utils.hpp"
#include "person.hpp"
#include "room.hpp"

using namespace std;

void Manager::signup()
{
    string username, password, purse, phone, address;
    username = Utils::get_param(line, "username");
    password = Utils::get_param(line, "password");
    purse = Utils::get_param(line, "purse");
    phone = Utils::get_param(line, "phone");
    address = Utils::get_param(line, "address");
    check_exist_username(username);
    all_users.push_back(new User(last_id++, username, password, stoi(purse), phone, address));
    throw Error(231);
}

void Manager::check_exist_username(string username)
{
    for(auto person : all_users)
        if (person->has_this_username(username))
            throw Error(451);
}

void Manager::signin()
{
    string username, password;
    username = Utils::get_param(line, "username");
    password = Utils::get_param(line, "password");
    find_signed_in_user(username, password);
    throw Error(230);
}

void Manager::find_signed_in_user(string username, string password)
{
    for(auto person : all_users)
        if (person->check_info(username, password))
        {
            users_fd_map[user_fd] = person;
            return;
        }
    throw Error(430);
}

void Manager::view_user_information()
{
    throw Message(users_fd_map[user_fd]->get_info("\n"));
}

void Manager::view_all_users()
{
    ostringstream info;
    for(auto user : all_users)
        info << user->get_info(" || ");
    throw Message(info.str());
}

void Manager::view_rooms_information_for_user()
{
    int filter_empty_status;
    filter_empty_status = (Utils::get_param(line, "filter_empty", false) == "") ? 
                            EmptyFilterStatus::DISABLE : EmptyFilterStatus::ENABLE;
    
    throw Message(get_rooms_info(false, filter_empty_status));
}

void Manager::view_rooms_information_for_admin()
{
    throw Message(get_rooms_info(true, EmptyFilterStatus::DISABLE));
}

string Manager::get_rooms_info(bool is_admin, int filter_empty_status)
{
    ostringstream info;
    info << "----------------------------------------------------------------------------------------------" <<endl;
    for(auto room : all_rooms)
        info << room->get_info(today, is_admin, filter_empty_status);
    info << "----------------------------------------------------------------------------------------------" << endl;
    return info.str();
}

void Manager::book()
{
    string RoomNum, NumOfBeds, CheckInDate, CheckOutDate;
    RoomNum = Utils::get_param(line, "RoomNum");
    NumOfBeds = Utils::get_param(line, "NumOfBeds");
    CheckInDate = Utils::get_param(line, "CheckInDate");
    CheckOutDate = Utils::get_param(line, "CheckOutDate");
    book_with_options(stoi(RoomNum), stoi(NumOfBeds), CheckInDate, CheckOutDate);
    throw Error(110);
}

void Manager::book_with_options(int RoomNum,int NumOfBeds, string CheckInDate, string CheckOutDate)
{
    int room_index = find_room(RoomNum);
    all_rooms[room_index]->add_new_rent(NumOfBeds, CheckInDate, CheckOutDate, 
                                        users_fd_map[user_fd], today);
    all_rooms[room_index]->update_room(today);
}

void Manager::show_reservations()
{
    ostringstream list;
    list << "----------------------------------------------------------------------------------------------" << endl
         << "today: " << today->day << '-' << today->month << '-' << today->year << endl
         << "your money: " << users_fd_map[user_fd]->get_wallet() << endl
         << "your_reservations:" << endl;
    for(auto room : all_rooms)
        list << room->check_reserved_and_get_info(users_fd_map[user_fd]->get_id());
    list << "----------------------------------------------------------------------------------------------" <<endl;
    throw Message(list.str());
}

void Manager::cancel()
{
    string RoomNum, Num;
    RoomNum = Utils::get_param(line, "RoomNum");
    Num = Utils::get_param(line, "Num");
    int room_index = find_room(stoi(RoomNum)), id = users_fd_map[user_fd]->get_id();
    all_rooms[room_index]->cancel_book(id, stoi(Num), today);
    throw Error(106);
}

void Manager::pass_day()
{
    string value;
    value = Utils::get_param(line, "value");
    today->pass_day(value);
    update_all_rooms();
    throw Error(110);
}

void Manager::update_all_rooms()
{
    for(auto room : all_rooms)
        room->update_room(today);
}

void Manager::edit_information_for_user()
{
    string new_password, phone, address;
    new_password = Utils::get_param(line, "new_password", false);
    phone = Utils::get_param(line, "phone", false);
    address = Utils::get_param(line, "address", false);
    users_fd_map[user_fd]->change_info(new_password, phone, address);
    throw Error(312);
}

void Manager::edit_information_for_admin()
{
    string new_password, phone, address;
    new_password = Utils::get_param(line, "new_password", false);
    users_fd_map[user_fd]->change_info(new_password);
    throw Error(312);
}

void Manager::leaving_room_for_user()
{
    string value;
    value = Utils::get_param(line, "value");
    int room_index = find_room(stoi(value)), id = users_fd_map[user_fd]->get_id();
    all_rooms[room_index]->leave_room(id, today);
    throw Error(413);
}

void Manager::leaving_room_for_admin()
{
    string value, new_capacity;
    value = Utils::get_param(line, "value");
    new_capacity = Utils::get_param(line, "new_capacity");
    int room_index = find_room(stoi(value));
    all_rooms[room_index]->eject(stoi(new_capacity), today);
    all_rooms[room_index]->update_room(today);
    throw Error(110);
}

void Manager::rooms()
{
    string room_command_request;
    room_command_request = Utils::get_param(line, "room_command_request");
    route_command(room_command_request, rooms_command_list);
}

void Manager::rooms_add()
{
    string RoomNum, Max_Capacity, Price;
    RoomNum = Utils::get_param(line, "RoomNum");
    Max_Capacity = Utils::get_param(line, "Max_Capacity");
    Price = Utils::get_param(line, "Price");
    check_room_is_not_exist(RoomNum);
    all_rooms.push_back(make_shared<Room>(stoi(RoomNum), RoomStatus::EMPTY, stoi(Price), stoi(Max_Capacity),
                                          stoi(Max_Capacity), vector<shared_ptr<Rent>>{}));
    throw Error(104);
}

void Manager::check_room_is_not_exist(string RoomNum)
{
    for (auto room : all_rooms)
        if(room->has_room_num(stoi(RoomNum)))
            throw Error(111);
}

void Manager::rooms_modify()
{
    string RoomNum, new_Max_Capacity, new_Price;
    RoomNum = Utils::get_param(line, "RoomNum", false);
    new_Max_Capacity = Utils::get_param(line, "Max_Capacity", false);
    new_Price = Utils::get_param(line, "Price", false);
    int room_index = find_room(stoi(RoomNum));
    all_rooms[room_index]->change_info(new_Max_Capacity, new_Price);
    throw Error(105);
}

void Manager::rooms_remove()
{
    string RoomNum;
    RoomNum = Utils::get_param(line, "RoomNum");
    int room_index = find_room(stoi(RoomNum));
    check_room_is_empty(room_index);
    all_rooms.erase(all_rooms.begin() + room_index);
    throw Error(106);
}

void Manager::check_room_is_empty(int room_index)
{
    if(all_rooms[room_index]->has_user(today))
        throw Error(109);
}

int Manager::find_room(int room_num)
{
    for(unsigned int i = 0; i < all_rooms.size(); i++)
        if(all_rooms[i]->has_room_num(room_num))
            return i;
    throw Error(101);
}

void Manager::logout()
{
    users_fd_map[user_fd] = nullptr;
    throw Error(201);
}