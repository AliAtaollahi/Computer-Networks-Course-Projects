#include <iostream>

#include "Utils.hpp"
#include "error.hpp"
#include "room.hpp"

using namespace std;

string Utils::parse_line(string str, char del) 
{
    str.append(" ");
    string res = "";
    istringstream sstream(str);
    getline(sstream, res, del);
    str = str.substr(res.size() + 1);
    return res;
}

string Utils::get_param(string line, string key, bool required) 
{
    vector<string> words = make_list_from_line(line, ' ');
    auto it = find(words.begin(), words.end(), key);
    if (it == words.end())
    {
        if (required)
            throw Error(503);
        else
            return "";
    }
    auto index = distance(words.begin(), it);
    return words[index + 1];
}

vector<string> Utils::make_list_from_line(string line, char divider) 
{
    vector<string> dividedLine;
    string argument;
    istringstream arguments(line);
    while (getline(arguments, argument, divider)) {
        if (argument == "")
            continue;
        dividedLine.push_back(argument);
    }
    return dividedLine;
}

DateConfig Utils::convert_to_DataConfig(string str)
{
    str.push_back('-');
    istringstream arguments(str);
    string day, month, year;
    getline(arguments, day, '-');
    getline(arguments, month, '-');
    getline(arguments, year, '-');
    return DateConfig(stoi(day), stoi(month), stoi(year));
}

std::string Utils::convert_DataConfig_to_string(DateConfig data_config)
{
    ostringstream info;
    info << data_config.day << '-' << data_config.month << '-' << data_config.year;  
    return info.str();
}

long int Utils::calc_distance(DateConfig* date)
{
    return date->year*360 + date->month*30 + date->day;
}

long int Utils::calc_distance(DateConfig date)
{
    return date.year*360 + date.month*30 + date.day;
}

std::string Utils::get_input()
{
    string message;
    char command[MAX_STRING_SIZE] = {0};
    int last_char = read(STD_IN, command, MAX_STRING_SIZE);
    command[last_char - 1] = '\0';
    message = command;
    return message;
}
