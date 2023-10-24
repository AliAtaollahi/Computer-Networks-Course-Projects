#pragma once

#include <string>
#include <vector>

class Cli
{
public:
    Cli();
    void handle_command(int server_fd);
    void notify(std::string response);
    void route_command_number(std::string message, int server_fd);
    void check_special_commands(int& command_num, std::string& options_line, int server_fd);
    void check_find_command(int index);
    void print_help(std::string help_message);
    void show_commands_list();
    std::string make_message_based_on_required_options_protocol(std::vector<std::string> options, std::string options_line="");
    std::string make_message_based_on_not_required_options_protocol(std::vector<std::string> options, std::string options_line="");
    void clear();
private:
    int menu_mode;
};