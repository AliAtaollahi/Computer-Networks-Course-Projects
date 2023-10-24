#pragma once

#include "classes.hpp"
#include <vector>
#include <string>

class AsRelation
{
private:
    int our_id_;
    int peer_id_;
    int role_;
    Link* link_;
public:
    AsRelation(int our_id, int peer_id, Link* link, int role);
    void send(Message* m);
    int get_role();
};
