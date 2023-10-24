# In the Name of God

*Mohammad Mohajel Sadegi - 810199483*

*Ali Ataollahi - 810199461*

# Hotel Managment System

In this project we are creating a hotel managment system called **Khane Ghamar Khanom** as our first Network course project.

We used socket programming and goal is to start a server which responeds to diffrent clients.

You can findout more about details in [Description](#description) part

# Description

In general, we have two client and server folder which will act seperatly from eachother

# Classes

## Client
It consists of these classes

Our main class is class client that we instantiate in main function
```c++
// main.cpp
int main(int argc, char const *argv[]) 
{
    Client client;
    client.connect_and_send_requests_to_server();

    return 0;
}
```

It contains these methods
```c++
void connect_and_send_requests_to_server();
void connect_to_server(int port, const char* ip);
void set_fd_set();
void send_requests();
```
*connect_and_send_requests_to_server* method basically calls other 3 and after setting fd and connecting to server, it goes throw a while loop in last method and uses select to handle input both from input buffer and client

```c++
void Client::send_requests()
{
    while (1) 
    {
        working_set = master_set;
        select(max_sd + 1, &working_set, NULL, NULL, NULL);
        for(int i = 0; i <= max_sd; i++) 
        {
            if (FD_ISSET(i, &working_set)) 
            { 
                if (i == STD_IN) //input buffer
                {
                    cli.handle_command(server_fd);
                }
                else if (i == server_fd) //server response
                {
                    // .... handling those
                }
                else
                    //...
            }
        }
    }
}
```

In order to see how **select** works go to [How Selecet Works](#how-selecet-works)

```c++
class Cli
{
public:
    Cli();
    void handle_command(int server_fd);
    void notify(std::string response);
    void route_command_number(std::string message, int server_fd);
    void check_special_commands(int& command_num, std::string& options_line, int server_fd);
    void check_find_command(int index);
    void print_help(std::string help_message);
    void show_commands_list();
    std::string make_message_based_on_required_options_protocol(std::vector<std::string> options, std::string options_line="");
    std::string make_message_based_on_not_required_options_protocol(std::vector<std::string> options, std::string options_line="");
    void clear();
private:
    int menu_mode;
};
```
Client uses *Cli* class. this class handles interface between client and input buffer (using handle command method) and between client and server (using notify method)



## Server

It consists of these classes

Our main class is class server that we instantiate in main function

```c++
// main.cpp
int main(int argc, char const *argv[]) 
{
    Manager manager;
    Server server(& manager);
    server.start_and_run_server();
    return 0;
}
```

```c++
class Server
{

public:
    Server(Manager *manager);
    void start_and_run_server();
    void set_fd_set();
    void run_server();

private:
    Manager *manager;
    int setup_server(int port); 
    int accept_client(int server_fd);
};
```

What server does is that it only establishes and manages connections between and from clients and server 

it does that using select to handle multipule clients simultanuously.

```c++
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
                    // ...
                    manager->add_new_client(new_client_fd);
                }

                else if (i == STD_IN) //input buffer
                {
                    // ...
                }
                
                else // client sending msg
                { 
                    char command[MAX_STRING_SIZE] = {0};
                    if (recv(i , command, MAX_STRING_SIZE, 0) == 0) 
                    { // EOF
                        // connction closed ....
                        continue;
                    }
                    // respond to client ...
                }
            }
        }
    }
```

after setting fd and connecting to server, it goes throw a while loop in *run_server* method

In order to see how **select** works go to [How Selecet Works](#how-selecet-works)
___

But *Manger* is the one that sits behind and computes everything.

its main method that server calls:

```c++
string Manager::handle_command_and_get_response(int fd, string command)
{
    line = command;
    user_fd = fd;
    string command_key = Utils::parse_line(command, ' ');
    try
    {
        auto command_list = get_command_list(user_fd);
        route_command(command_key, command_list);
    } catch (Error& error) {
        return error.what();
    } catch (Message& message) {
        return message.what();
    }
    return "";
}
```

this class contains a function for each command that clients can enter and we have a mapping (actully 3 mapping unregistered_user_list user_command_list admin_command_list) between command keys and these functions to call(very neat indeed)

```c++
const std::unordered_map<std::string, func_ptr> user_command_list = {
    {"view_user_information", &Manager::view_user_information},
    {"view_rooms_information", &Manager::view_rooms_information_for_user},
    {"book", &Manager::book},
    {"show_reservations", &Manager::show_reservations},
    {"cancel", &Manager::cancel},
    {"edit_information", &Manager::edit_information_for_user},
    {"leaving_room", &Manager::leaving_room_for_user},
    {"logout", &Manager::logout},
};
```

And we have a simple *Error* class and *Message* which we throw and by calling thir **what()** method we can get our desired info (to return as response for example)


# Writing In Json File

We used cppjson to write users and rooms info in files and restore them again.

## Getting Started

In order to use cppjson, you can install it with this command (fedora 33 or higher)

    sudo dnf install jsoncpp-devel


It is also available in ubuntu which mt teammate uses but we encountered a problem here:

## What To Include

in fedora you need to include

    #include <json/json.h>

and in ubuntu

    # include <jsoncpp/json/json.h>

we solved this issue by putting a **#if _has_include**

```c++
#if __has_include(<jsoncpp/json/json.h>)
# include <jsoncpp/json/json.h>
#elif __has_include(<json/json.h>)
#include <json/json.h>
#endif
```
win win strategy!

## How We Used It

*FileHandler* class used cpp json and gave us methods to read/write from/to config.json, rooms.json, admin.json, user.json files in database folder.(totally 8 methods)

lets look at simplest read and write example of methods

```c++

bool FileHandler::write_in_admin_user_file(vector<AdminUserInfo> info, string path)
{
    Json::Value root;
    ofstream output_file(path, ifstream::binary);
    for(auto user_info : info)
    {
        Json::Value user; 

        user["id"] = user_info.id;
        user["user_name"] = user_info.user_name;
        user["password"] = user_info.password;
        user["admin"] = user_info.admin;

        root.append(user);
    }
    output_file << root << endl;
    return true;
}
```
in order to write in json file we create a base json value called root and append other json values to it(here adding user) 

```c++
vector<AdminUserInfo> FileHandler::read_from_admin_user_file(string path)
{
    Json::Value root;
    std::ifstream input_file(path, std::ifstream::binary);
    vector<AdminUserInfo> info;
    input_file >> root;

    for(auto user = root.begin(); user != root.end() ; user++)
    {
        AdminUserInfo user_info;

        user_info.id = (*user)["id"].asInt();
        user_info.user_name = (*user)["user_name"].asString();
        user_info.password = (*user)["password"].asString();
        user_info.admin = (*user)["admin"].asBool();

        info.push_back(user_info);
    }
    return info;
}
```
when reading, first, we read all file to file straem and read that to root json::value.

then we create a vector of structs and read desired info one by one to it and return it.

# How Selecet Works

select() allows a program to monitor multiple file descriptors,
       waiting until one or more of the file descriptors become "ready"
       for some class of I/O operation (e.g., input possible).  A file
       descriptor is considered ready if it is possible to perform a
       corresponding I/O operation (e.g., read(2), or a sufficiently
       small write(2)) without blocking.

   File descriptor sets
       The principal arguments of select() are three "sets" of file
       descriptors (declared with the type fd_set), which allow the
       caller to wait for three classes of events on the specified set
       of file descriptors.  Each of the fd_set arguments may be
       specified as NULL if no file descriptors are to be watched for
       the corresponding class of events.

       Note well: Upon return, each of the file descriptor sets is
       modified in place to indicate which file descriptors are
       currently "ready".  Thus, if using select() within a loop, the
       sets must be reinitialized before each call.

       The contents of a file descriptor set can be manipulated using
       the following macros:

       FD_ZERO()
              This macro clears (removes all file descriptors from) set.
              It should be employed as the first step in initializing a
              file descriptor set.

       FD_SET()
              This macro adds the file descriptor fd to set.  Adding a
              file descriptor that is already present in the set is a
              no-op, and does not produce an error.

       FD_CLR()
              This macro removes the file descriptor fd from set.
              Removing a file descriptor that is not present in the set
              is a no-op, and does not produce an error.

       FD_ISSET()
              select() modifies the contents of the sets according to
              the rules described below.  After calling select(), the
              FD_ISSET() macro can be used to test if a file descriptor
              is still present in a set.  FD_ISSET() returns nonzero if
              the file descriptor fd is present in set, and zero if it
              is not.


# Test

![My Image](./test-pictures/pic1.png)
![My Image](./test-pictures/1.png)
![My Image](./test-pictures/2.png)
![My Image](./test-pictures/3.png)
![My Image](./test-pictures/4.png)
![My Image](./test-pictures/5.png)
![My Image](./test-pictures/6.png)
![My Image](./test-pictures/7.png)
![My Image](./test-pictures/8.png)
![My Image](./test-pictures/9.png)
![My Image](./test-pictures/10.png)
![My Image](./test-pictures/11.png)
![My Image](./test-pictures/12.png)
![My Image](./test-pictures/13.png)
![My Image](./test-pictures/14.png)
![My Image](./test-pictures/15.png)
![My Image](./test-pictures/16.png)
![My Image](./test-pictures/17.png)
![My Image](./test-pictures/18.png)
![My Image](./test-pictures/19.png)
![My Image](./test-pictures/20.png)
![My Image](./test-pictures/21.png)
![My Image](./test-pictures/22.png)
![My Image](./test-pictures/23.png)
![My Image](./test-pictures/24.png)
![My Image](./test-pictures/25.png)
![My Image](./test-pictures/26.png)
![My Image](./test-pictures/27.png)
![My Image](./test-pictures/28.png)
![My Image](./test-pictures/29.png)
![My Image](./test-pictures/30.png)
![My Image](./test-pictures/31.png)
![My Image](./test-pictures/32.png)
![My Image](./test-pictures/33.png)
![My Image](./test-pictures/34.png)
![My Image](./test-pictures/35.png)
![My Image](./test-pictures/36.png)
![My Image](./test-pictures/37.png)
![My Image](./test-pictures/38.png)
![My Image](./test-pictures/39.png)
![My Image](./test-pictures/40.png)
![My Image](./test-pictures/41.png)
