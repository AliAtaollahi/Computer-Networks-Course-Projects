#pragma once

#include <cstdlib>
#include <time.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include <unordered_map>
#include <vector>

#include "ns3-includes.hh"
#include "paket-sender.hh"
#include "manual.hh"

using namespace ns3;
using namespace std;


class master : public Application
{
public:
    master (uint16_t port, unordered_map<int, Ipv4InterfaceContainer>& interfaces_);
    virtual ~master ();
private:
    virtual void StartApplication (void);
    void HandleRead (Ptr<Socket> socket);
    unordered_map<int, Ipv4InterfaceContainer> interfaces;
    uint16_t port;
    
    Ptr<Socket> socket;
    vector<Ptr<Socket>> mappers_sockets;
    vector<InetSocketAddress> mappers_sockets_address;
};

master::master (uint16_t port, unordered_map<int, Ipv4InterfaceContainer>& interfaces_)
        : interfaces(interfaces_),
          port (port),
          mappers_sockets({}),
          mappers_sockets_address({})
{
    std::srand (time(0));
}

master::~master ()
{
}

void master::StartApplication (void)
{
    // bind to client
    socket = Socket::CreateSocket (GetNode (), UdpSocketFactory::GetTypeId ());
    InetSocketAddress local = InetSocketAddress (interfaces[InterfacesNum::MASTER].GetAddress(0), port);
    socket->Bind (local);
    socket->SetRecvCallback (MakeCallback (&master::HandleRead, this));

    // connecting to mappers
    for (int i = 0; i < MAPPERS_NUM; i++)
    {
        mappers_sockets.push_back(Socket::CreateSocket (GetNode (), TcpSocketFactory::GetTypeId ()));
        mappers_sockets_address.push_back(InetSocketAddress(interfaces[InterfacesNum::MAPPER].GetAddress(i), port));
        if (mappers_sockets[i]->Connect(mappers_sockets_address[i]) == -1)
        {
            cout << KRED << "_CANT_CONNECT_TO_MAPPER_1_=" << endl;
            exit(EXIT_FAILURE);
        }
    }
}

void master::HandleRead (Ptr<Socket> socket)
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

        send_packet(mappers_sockets[0], buffer, "\t(TCP)master->mapper0", MAPPER_0_COLOR);
        send_packet(mappers_sockets[1], buffer, "\t(TCP)master->mapper1", MAPPER_1_COLOR);
        send_packet(mappers_sockets[2], buffer, "\t(TCP)master->mapper2", MAPPER_2_COLOR);
    }
}