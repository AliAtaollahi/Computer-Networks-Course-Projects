#include "AS.hpp"
#include "AsRelation.hpp"
#include "manual.hpp"

using namespace std;

vector<string> make_list_from_line(string line, char divider) 
{
    line.push_back(' ');
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

AS::AS(int id, vector<string> owned_ips)
{
    id_ = id;
    owned_ips_ = owned_ips;
    connected_AS_ = {};
    path_ips_ = {};
    auto_advertise_ = false;
}

int AS::get_id()
{
    return id_;
}

void AS::receive(Message* m, int sender_id)
{
    if(m->is_advertise)
    {
        if(is_hijack(m, sender_id))
            return;
        vector<int> new_traverse_route = m->traversed_route;
        new_traverse_route.push_back(id_);
        if(!is_new_subnet(m->range_ip, new_traverse_route))
            return;
        if(auto_advertise_) {} // must be compelete
    }
}

bool AS::is_hijack(Message* m, int sender_id)
{
    return false; // must be compelete
}

bool AS::is_new_subnet(vector<string> subnets, vector<int>& new_traverse_route)
{
    if(subnets == owned_ips_)
        return false;
    
    for (auto x : subnets)
    {
        if(path_ips_.find(x) != path_ips_.end())
        {
            if (find(path_ips_[x].begin(), 
                     path_ips_[x].end(), 
                     new_traverse_route) == path_ips_[x].end())
                path_ips_[x].push_back(new_traverse_route);
        }
        else 
        {
            path_ips_[x].clear();
            path_ips_[x] = {new_traverse_route};
        }
    }
    return true;
}

void AS::add_link(AsRelation* new_link)
{
    connected_AS_.push_back(new_link);
}

void AS::get_new_command(std::string command)
{
    vector<string> args = make_list_from_line(command, ' ');
    if(args[0] == "ads_self") ads_self();
    else if(args[0] == "ads_all") ads_all();
    else if(args[0] == "get_route") {}//get_route();
    else if(args[0] == "hijack_routing") {}//hijack_routing();
    else if(args[0] == "auto_ads_on") {}//set_ads_on();
}

// advertise your ips
void AS::ads_self()
{
    for(auto x : connected_AS_)
        ads(owned_ips_, x, {});
}

// advertise all paths you know (include yourself ips)
void AS::ads_all()
{
    ads_self();
    for(auto x : connected_AS_)
    {
        if(x->get_role() == roles::COSTUMER)
            continue; // this can be changed with pollicy
        for (auto xx : path_ips_)
            for(auto xxx : path_ips_[xx.first])
                ads({xx.first}, x, xxx);
    }
}

void AS::ads(std::vector<std::string> subnets, 
             AsRelation* relation, 
             std::vector<int> traversed_routes)
{
    if(traversed_routes.size() == 0)
        traversed_routes = {id_};

    Message* new_message = new Message{true,
                                       subnets,
                                       traversed_routes};
    relation->send(new_message);
}

void AS::print_subnets()
{
    cout << "as" << id_ << " paths:" << endl;
    for (auto x : path_ips_)
        cout << x.first << ' ';
    cout << endl << endl;
}