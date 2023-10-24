#pragma once

#include <vector>
#include <string>

class AS;
class AsRelation;
class Link;

struct Message
{
    bool is_advertise;
    std::vector<std::string> range_ip;
    std::vector<int> traversed_route;
};