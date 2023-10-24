#pragma once

#include <string>
#include <vector>

namespace CliUtils 
{
    void print_init_command();
    std::string get_input(bool have_to_print_init=true);
    void check_is_num(std::string a);
    std::vector<std::string> make_list_from_line(std::string line, char divider);
    std::string parse_line(std::string str, char del);
    std::string get_param(std::string line, std::string key, bool required);
    class Error
    {
    public:
        Error(std::string text_);
        std::string what();
    private:
        std::string text;
    };
}