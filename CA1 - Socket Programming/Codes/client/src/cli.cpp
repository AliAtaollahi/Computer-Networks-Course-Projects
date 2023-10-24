#include <unistd.h>
#include <arpa/inet.h>
#include <cstring>
#include <iostream>
#include <stdlib.h>

#include "cli.hpp"
#include "manual.hpp"
#include "CliUtils.hpp"

using namespace std;

map<int, string> commands_help::desired_map = commands_help::create_map();
map<int, string> commands_list::desired_map = commands_list::create_map();
map<int, string> commands_names::desired_map = commands_names::create_map();
map<int, pair<int, vector<string>>> commands_with_options::desired_map = commands_with_options::create_map();

Cli::Cli()
{
    menu_mode = MenuMode::not_registered_menu;
}

void Cli::handle_command(int server_fd)
{
    string message = CliUtils::get_input(false);
    try
    {
        route_command_number(message, server_fd);
    }
    catch (CliUtils::Error e)
    {
        cout << e.what() << endl;
        CliUtils::print_init_command();
    }
}

void Cli::notify(std::string response)
{
    int old_mode = menu_mode;
    menu_mode = (response.find("USER_LOGED_IN") != std::string::npos) ? MenuMode::registered_menu : 
                (response.find("USER_LOGED_OUT_SUCCESSFULLY") != std::string::npos) ? MenuMode::not_registered_menu : 
                 menu_mode;
    if(old_mode != menu_mode)
        show_commands_list();
}

void Cli::show_commands_list()
{
    cout << commands_list::desired_map[menu_mode];
}

void Cli::route_command_number(string message, int server_fd)
{
    if(message.find("cls") != std::string::npos) 
    {
        clear();
        return;
    }

    string options_line = "", options = "";
    CliUtils::check_is_num(message);
    int command_num = stoi(message);
    check_special_commands(command_num, options_line, server_fd);
    auto command_options = commands_with_options::desired_map[command_num];
    check_find_command(command_options.first);
    if(!command_options.second.empty())
    {
        if(options_line == "" && !command_options.second.empty()) print_help(commands_help::desired_map[command_num]);
        options = (command_options.first == CommandMode::options_required) ? 
                   make_message_based_on_required_options_protocol(command_options.second, options_line) : 
                   make_message_based_on_not_required_options_protocol(command_options.second, options_line);
    }
    string formated_command = commands_names::desired_map[command_num] + " " + options;
    send(server_fd, formated_command.c_str(), strlen(formated_command.c_str()), 0);
}

void Cli::check_special_commands(int& command_num, string& options_line, int server_fd)
{
    if(command_num == CommandsNum::Rooms)
    {
        print_help(commands_help::desired_map[CommandsNum::Rooms]);
        options_line = CliUtils::get_input();
        string first_part_of_command = CliUtils::parse_line(options_line, ' ');
        command_num = (first_part_of_command == "add") ? CommandsNum::RoomCommand::Rooms_add :
                      (first_part_of_command == "modify") ? CommandsNum::RoomCommand::Rooms_modify :
                      (first_part_of_command == "remove") ? CommandsNum::RoomCommand::Rooms_remove :
                       command_num;
        if(first_part_of_command == "modify")
            options_line = "room_command_request " + options_line;
    }
    else if(command_num == CommandsNum::Canceling && menu_mode == MenuMode::registered_menu)
    {
        string list_reservations_command = "show_reservations";
        send(server_fd, list_reservations_command.c_str(), strlen(list_reservations_command.c_str()), 0);
    }
}

void Cli::check_find_command(int index)
{
    if(index == 0)
        throw CliUtils::Error("CliErr -> Use right number\n");
}

void Cli::print_help(std::string help_message)
{
    cout << endl
         << "----------------------------------------------------------------------------------------------" << endl
         << help_message
         << "----------------------------------------------------------------------------------------------" << endl
         << endl;
}

string Cli::make_message_based_on_required_options_protocol(vector<string> options, string options_line)
{
    string input_options = (options_line == "") ? CliUtils::get_input() : options_line;
    auto input_options_ = CliUtils::make_list_from_line(input_options, ' ');
    if(input_options_.size() != options.size())
        throw CliUtils::Error("CliErr -> Use right options\n");

    ostringstream info;
    for (unsigned int i = 0; i < options.size(); i++)
        info << options[i] << " " << input_options_[i] << " ";
    return info.str();
}

string Cli::make_message_based_on_not_required_options_protocol(vector<string> options, string options_line)
{
    string input_options = (options_line == "") ? CliUtils::get_input() : options_line, param;
    ostringstream info;
    for (auto option : options)
    {
        param = CliUtils::get_param(input_options, option, false);
        if(param != "")
            info << option << " " << param << " ";
    }
    return info.str();
}

void Cli::clear()
{
    system("clear");
    show_commands_list();
    CliUtils::print_init_command();
}