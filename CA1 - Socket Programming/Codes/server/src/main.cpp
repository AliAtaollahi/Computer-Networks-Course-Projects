// in the name of God

#include "manual.hpp"
#include "manager.hpp"
#include "server.hpp"

using namespace std;

map<int,string> Info::status =  Info::create_map();

int main(int argc, char const *argv[]) 
{
    Manager manager;
    Server server(& manager);
    server.start_and_run_server();
    return 0;
}
