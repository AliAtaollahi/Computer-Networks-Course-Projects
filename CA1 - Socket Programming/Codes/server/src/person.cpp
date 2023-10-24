#include <algorithm>

#include "person.hpp"
#include "manager.hpp"
#include "error.hpp"
#include "Utils.hpp"

using namespace std;

Person::Person(int id_,string username_, string password_)
    :
    id(id_),
    username(username_),
    password(password_)
{
}

bool Person::check_info(string username_, string password_) 
{
    return (username == username_ && password == password_);
}

bool Person::has_this_username(string username_)
{
    return username_ == username; 
}

bool Person::has_this_id(int id_)
{
    return id == id_;
}

void Person::change_info(string new_password, string new_phone, string new_address) {}
void Person::change_info(string new_password) {}
int Person::get_wallet() { return 0; }
void Person::checkout(int price) {}
void Person::earn(int price) {}

int Person::get_id()
{
    return id;
}

User::User(int id_,string username_, string password_, int purse_, string phone_number_, string address_) 
    : 
    Person(id_, username_, password_) 
{
    purse = purse_;
    phone_number = phone_number_;
    address = address_;
    is_admin = false;
}

unordered_map<string, func_ptr> User::get_command_list()
{
    return user_command_list;
}

string User::get_info(string del)
{
    ostringstream info;
    info << "id: " << to_string(id) << del 
         << "username: " << username << del
         << "password: " << password << del
         << "is_admin: " << to_string(is_admin) << del
         << "purse: " << purse << del
         << "phone_number: " << phone_number << del
         << "address: " << address << endl;
    return info.str();
}

void User::change_info(string new_password, string new_phone, string new_address) 
{
    if (password == new_password || 
        phone_number == new_phone ||
        address == new_address)
        throw Error(503);

    password = (new_password == "") ?  password : new_password;
    phone_number = (new_phone == "") ?  phone_number : new_phone;
    address = (new_address == "") ?  address : new_address;
}

int User::get_wallet()
{
    return purse;
}

void User::checkout(int price)
{
    if(purse < price)
        throw Error(108);
    purse -= price;
}

void User::earn(int price)
{
    purse += price;
}

FileDataContainers::UserInfo User::get_data_for_write()
{
    FileDataContainers::UserInfo user_info;
    user_info = {id, purse, username, password, phone_number, address, is_admin};
    return user_info;
}

Admin::Admin(int id_,string username_, string password_) 
    : 
    Person(id_, username_, password_) 
{
    is_admin = true;
}


unordered_map<string, func_ptr> Admin::get_command_list()
{
    return admin_command_list;
}

string Admin::get_info(string del)
{
    ostringstream info;
    info << "id: " << to_string(id) << del 
         << "username: " << username << del
         << "password: " << password << del
         << "is_admin: " << to_string(is_admin) << endl;
    return info.str();
}

void Admin::change_info(string new_password)
{
    if (password == new_password)
        throw Error(503);
        
    password = new_password;
}

FileDataContainers::UserInfo Admin::get_data_for_write()
{
    FileDataContainers::UserInfo user_info;
    user_info.id = id;
    user_info.admin = is_admin;
    user_info.password = password;
    user_info.user = username;
    return user_info;
}