#include <unistd.h>
#include <string>
#include <algorithm>
#include <cstring>

#include "CliUtils.hpp"
#include "manual.hpp"

using namespace std;

void CliUtils::print_init_command()
{
    write(STD_OUT, "\x1B[34m>> \x1B[37m" , strlen("\x1B[34m>> \x1B[37m"));
}

string CliUtils::get_input(bool have_to_print_init)
{
    if(have_to_print_init)
        CliUtils::print_init_command();
    string message;
    char command[MAX_STRING_SIZE] = {0};
    int last_char = read(STD_IN, command, MAX_STRING_SIZE);
    command[last_char - 1] = '\0';
    message = command;
    return message;
}

void CliUtils::check_is_num(string a)
{
    if(a == "")
        throw CliUtils::Error("");
    if(! all_of(a.begin(), a.end(), ::isdigit))
        throw CliUtils::Error("CliErr -> Enter number not string\n");
}

vector<string> CliUtils::make_list_from_line(string line, char divider) 
{
    vector<string> dividedLine;
    string argument;
    istringstream arguments(line);
    while (getline(arguments, argument, divider)) 
    {
        if (argument == "")
            continue;
        dividedLine.push_back(argument);
    }
    return dividedLine;
}

string CliUtils::parse_line(string str, char del) 
{
    str.append(" ");
    string res = "";
    istringstream sstream(str);
    getline(sstream, res, del);
    str = str.substr(res.size() + 1);
    return res;
}

string CliUtils::get_param(string line, string key, bool required) 
{
    vector<string> words = make_list_from_line(line, ' ');
    auto it = find(words.begin(), words.end(), key);
    if (it == words.end())
    {
        if (required)
            throw CliUtils::Error("CliErr -> Use right options\n");
        else
            return "";
    }
    auto index = distance(words.begin(), it);
    return words[index + 1];
}

CliUtils::Error::Error(std::string text_)
    :
    text(text_)
{    
}

std::string CliUtils::Error::what()
{
    return text;
}