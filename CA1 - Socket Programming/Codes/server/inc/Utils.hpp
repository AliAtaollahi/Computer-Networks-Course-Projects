#pragma once

#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <iostream>
#include <unistd.h>

#include "class_names.hpp"

namespace Utils {
    std::string parse_line(std::string str, char del);
    std::vector<std::string> make_list_from_line(std::string line, char divider);
    std::string get_param(std::string line, std::string key, bool required=true);
    DateConfig convert_to_DataConfig(std::string str);
    std::string convert_DataConfig_to_string(DateConfig data_config);
    long int calc_distance(DateConfig* date);
    long int calc_distance(DateConfig date);
    std::string get_input();
}