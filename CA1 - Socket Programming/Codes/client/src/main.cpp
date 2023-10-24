// in the name of God

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <string.h> 
#include <signal.h>
#include <sys/ioctl.h>
#include <stdbool.h>
#include<unistd.h>

#include "manual.hpp"
#include "client.hpp"

int main(int argc, char const *argv[]) 
{
    Client client;
    client.connect_and_send_requests_to_server();

    return 0;
}