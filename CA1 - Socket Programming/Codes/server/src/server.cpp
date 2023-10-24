// In the Name of God

#include "server.hpp"
#include "manual.hpp"
#include "Utils.hpp"

#include <iostream>

using namespace std;

Server::Server(Manager *manager)
    :
    file_handler(FileHandler()),
    logger(Logger()),
    manager(manager),
    server_fd(NOT_CONNECTED),
    max_sd(0)
{
    FileDataContainers::config config = file_handler.read_config(Paths::CONFIG_SERVER_DATA_PATH);
    server_port = config.commandChannelPort;
    server_ip = config.hostName;
}

void Server::start_and_run_server() 
{
    server_fd = setup_server(server_port);
    set_fd_set();
    run_server();
}

void Server::set_fd_set()
{
    max_sd = server_fd;
    FD_ZERO(&master_set);
    FD_SET(server_fd, &master_set);
    FD_SET(STD_IN, &master_set);
}

void Server::run_server()
{
    while (1) 
    {
        working_set = master_set;
        select(max_sd + 1, &working_set, NULL, NULL, NULL);

        for(int i = 0; i <= max_sd; i++) 
        {
            if (FD_ISSET(i, &working_set)) 
            {    
                if (i == server_fd) // new clinet
                {  
                    int new_client_fd = accept_client(server_fd);
                    FD_SET(new_client_fd, &master_set);
                    if (new_client_fd > max_sd)
                        max_sd = new_client_fd;
                    manager->add_new_client(new_client_fd);
                }

                else if (i == STD_IN) //input buffer
                {
                    string server_input = Utils::get_input();
                    server_input.push_back(' ');
                    manager->set_today(server_input);
                }
                
                else // client sending msg
                { 
                    char command[MAX_STRING_SIZE] = {0};
                    if (recv(i , command, MAX_STRING_SIZE, 0) == 0) 
                    { // EOF
                        printf("%s***\t client fd = %d closed\t***%s\n",KCYN, i, KWHT);
                        close(i);
                        FD_CLR(i, &master_set);
                        continue;
                    }
                    // printf("client with fd = %d send %s \n", i, command);
                    logger.log("User", i, command);
                    string response = manager->handle_command_and_get_response(i, string(command));
                    logger.log("Server", i, response);
                    int status = send(i, response.c_str(), strlen(response.c_str()), 0);
                    if (status == 0)
                        printf("%s***\t could not send to client fd = %d\t***%s\n",KRED, i, KWHT);
                }
            }
        }
    }
}

int Server::setup_server(int port)
{
    struct sockaddr_in address;
    int server_fd;
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_fd < 0)
	{
		cerr << "Fail to create socket" << endl;
		exit(1);
	}

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(server_ip.c_str());
    address.sin_port = htons(port);

    if(bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
	{
		cerr << "Error while binding" << endl;
		exit(1);
	}
	else
		cout << "Socket binded" << endl;

	if(listen(server_fd, MAX_CONNECTIONS) < 0)
	{
		cerr << "Error in listening." << endl;;
		exit(1);
	}
	else
		cout << "Server is listening." << endl;

    return server_fd;
}

int Server::accept_client(int server_fd)
{
    int client_fd;
    struct sockaddr_in client_address;
    int address_len = sizeof(client_address);
    client_fd = accept(server_fd, (struct sockaddr *)&client_address, (socklen_t*) &address_len);
    return client_fd;
}