#pragma once

#include <cstdlib>
#include<time.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include <unordered_map>

#include "ns3-includes.hh"
#include "paket-sender.hh"
#include "manual.hh"

using namespace ns3;
using namespace std;

class mapper : public Application
{
public:
    mapper (uint16_t port, unordered_map<int, Ipv4InterfaceContainer>& interfaces_, int mapper_number);
    virtual ~mapper ();
private:
    virtual void StartApplication (void);
    void HandleRead (Ptr<Socket> socket);
    void HandleAccept(Ptr<Socket> sock, const Address & addr);
    unordered_map<int, Ipv4InterfaceContainer> interfaces;

    int mapper_number;
    string color;

    uint16_t port;
    Ptr<Socket> socket;
    Ptr<Socket> client_socket;
};

mapper::mapper (uint16_t port, unordered_map<int, Ipv4InterfaceContainer>& interfaces_, int mapper_number_)
        : 
        interfaces(interfaces_),
        port (port)
{
    std::srand (time(0));
    this->mapper_number = mapper_number_;
    switch (this->mapper_number)
    {
        case 0: this->color = MAPPER_0_COLOR;  break;
        case 1: this->color = MAPPER_1_COLOR;  break;
        case 2: this->color = MAPPER_2_COLOR;  break;
    }   
}

mapper::~mapper ()
{
}

void mapper::StartApplication (void)
{
    // bind to master
    Ptr<Socket> socket_tcp = Socket::CreateSocket (GetNode (), TcpSocketFactory::GetTypeId ());
    InetSocketAddress local = InetSocketAddress (interfaces[InterfacesNum::MAPPER].GetAddress(this->mapper_number), port);
    socket_tcp->Bind (local);
    socket_tcp->Listen();
    socket_tcp->SetAcceptCallback(MakeNullCallback<bool, Ptr< Socket >, const Address &> (),
                                  MakeCallback (&mapper::HandleAccept, this));

    // connect to client
    client_socket = Socket::CreateSocket (GetNode (), UdpSocketFactory::GetTypeId ());
    InetSocketAddress client_socket_address (interfaces[InterfacesNum::CLIENT].GetAddress(0), MAPPERS_PORT_INIT + this->mapper_number);

    if (client_socket->Connect(client_socket_address) == -1)
    {
        cout << KRED << "_CANT_CONNECT_CLIENT_" << endl;
        exit(EXIT_FAILURE);
    }   
}

void mapper::HandleAccept(Ptr<Socket> sock, const Address &addr)
{
    cout << "inside HandleAccept"<< endl;
    sock->SetRecvCallback(MakeCallback (&mapper::HandleRead, this));
}

void mapper::HandleRead (Ptr<Socket> socket)
{
    Ptr<Packet> packet;

    while ((packet = socket->Recv ()))
    {
        if (packet->GetSize () == 0)
        {
            break;
        }
        MyHeader destinationHeader;
        packet->RemoveHeader (destinationHeader);
        uint16_t buffer = destinationHeader.GetData();

        if (this->mapper_number == buffer % 3)
        {
            string msg = "\t\t(UDP)mapper" + to_string(this->mapper_number) 
                    + "->client '" + "' decrypt[" + to_string(buffer) + "] = " +  Codes::decrypt[buffer] +" ::";

            send_packet(client_socket, Codes::decrypt[buffer], msg, this->color);
        }
    }
}