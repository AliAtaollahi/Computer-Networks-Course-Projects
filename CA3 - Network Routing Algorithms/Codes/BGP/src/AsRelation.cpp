#include "AsRelation.hpp"
#include "Link.hpp"

using namespace std;

AsRelation::AsRelation(int our_id, int peer_id, Link* link, int role)
{
    our_id_ = our_id;
    peer_id_ = peer_id;
    link_ = link;
    role_ = role;
}

void AsRelation::send(Message* m)
{
    link_->send(m, our_id_);
}

int AsRelation::get_role()
{
    return role_;
}