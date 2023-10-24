#pragma once

#include <cstdlib>
#include<time.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include <iomanip> 
#include <unordered_map>

#include "ns3-includes.hh"
#include "packet-header.hh"
#include "manual.hh"

using namespace ns3;
using namespace std;

void send_packet(Ptr<Socket> socket, uint16_t data, string info = "", string color = KWHT)
{
    Ptr<Packet> packet = new Packet();
    MyHeader header;
    header.SetData(data);

    packet->AddHeader (header);

    string error_msg = "";
    if (socket->Send(packet) == -1) //error occured
        error_msg = "_ERROR_SENDING_";

    cout << std::setfill('.') << color << std::setw(24) << info << ": " << data << KWHT << endl << endl; 
}

void generate_initial_traffic (Ptr<Socket> socket, uint16_t data, string info = "")
{
    send_packet(socket, data, info);
    Simulator::Schedule (Seconds (0.1), &generate_initial_traffic, socket, rand() % 26, info);
}
