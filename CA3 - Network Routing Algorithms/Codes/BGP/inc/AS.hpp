#pragma once

#include "classes.hpp"

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <unordered_map>

class AS
{
private:
    int id_;
    std::vector<AsRelation*> connected_AS_;
    std::vector<std::string> owned_ips_;
    std::unordered_map<std::string, std::vector<std::vector<int>>> path_ips_;
    bool auto_advertise_;
public:
    AS(int id, std::vector<std::string> owned_ips);
    int get_id();
    void receive(Message* m, int sender_id);
    void add_link(AsRelation* new_link);
    void get_new_command(std::string command);
    void ads_self();
    void ads_all();
    void get_route();
    void hijack_routing();
    void set_ads_on();
    void ads(std::vector<std::string> subnets, AsRelation* relation, std::vector<int> traversed_routes);
    bool is_hijack(Message* m, int sender_id); 
    bool is_new_subnet(std::vector<std::string> subnets, std::vector<int>& new_traverse_route);
    void print_subnets();
};
