#pragma once

#include <string>
#include <unordered_map>
#include <sstream>

#include "class_names.hpp"
#include "manual.hpp"

class Person {
public:
    Person (int id_,std::string username_, std::string password_);
    virtual std::unordered_map<std::string, func_ptr> get_command_list() = 0;
    bool check_info(std::string username_, std::string password_);
    bool has_this_username(std::string username_);
    bool has_this_id(int id_);
    virtual std::string get_info(std::string del) = 0;
    virtual void change_info(std::string new_password, std::string new_phone, std::string new_address);
    virtual void change_info(std::string new_password);
    int get_id();
    virtual int get_wallet();
    virtual void checkout(int price);
    virtual void earn(int price);
    virtual FileDataContainers::UserInfo get_data_for_write() = 0;
protected:
    int id;
    std::string username;
    std::string password;
    bool is_admin;
};

class User : public Person {
public:
    User(int id_,std::string username_, std::string password_, int purse_, std::string phone_number_, std::string address_);
    std::unordered_map<std::string, func_ptr> get_command_list();
    std::string get_info(std::string del);
    void change_info(std::string new_password, std::string new_phone, std::string new_address);
    int get_wallet();
    void checkout(int price);
    void earn(int price);
    FileDataContainers::UserInfo get_data_for_write();
private:
    int purse;
    std::string phone_number;
    std::string address;
};

class Admin : public Person {
public:
    Admin(int id_,std::string username_, std::string password_);
    std::unordered_map<std::string, func_ptr> get_command_list();
    std::string get_info(std::string del);
    void change_info(std::string new_password);
    FileDataContainers::UserInfo get_data_for_write();
};
