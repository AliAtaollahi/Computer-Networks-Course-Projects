#pragma once

#include "classes.hpp"
#include <vector>
#include <string>

class Link
{
private:
    AS* first_as_;
    AS* second_as_;
public:
    Link(AS* first_as, AS* second_as);
    void send(Message* m, int sender_id);
};
