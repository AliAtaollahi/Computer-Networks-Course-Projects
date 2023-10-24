#include "Link.hpp"
#include "AS.hpp"

using namespace std;

Link::Link(AS* first_as, AS* second_as)
{
    first_as_ = first_as;
    second_as_ = second_as;
}

void Link::send(Message* m, int sender_id)
{
    if(sender_id == first_as_->get_id()) second_as_->receive(m, sender_id);
    else first_as_->receive(m, sender_id);
}
