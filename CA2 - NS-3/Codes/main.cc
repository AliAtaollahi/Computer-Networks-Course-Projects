#include <cstdlib>
#include<time.h>
#include <stdio.h>
#include <string>
#include <fstream>
#include <unordered_map>

#include "ns3-includes.hh"
#include "monitors.hh"
#include "packet-header.hh"
#include "master.hh"
#include "paket-sender.hh"
#include "client.hh"
#include "mapper.hh"
#include "manual.hh"

using namespace ns3;
using namespace std;

map<int,char> Codes::decrypt =  Codes::create_map();

NS_LOG_COMPONENT_DEFINE ("WifiTopology");

int main (int argc, char *argv[])
{
    double error = 0.000001;
    string bandwidth = "1Mbps";
    double duration = 10.0;

    srand(time(NULL));

    // some logs comment if not needed
    LogComponentEnable ("UdpEchoClientApplication", LOG_LEVEL_INFO);
    LogComponentEnable ("UdpEchoServerApplication", LOG_LEVEL_INFO);


    // craete node containers
    NodeContainer client_node_container;
    client_node_container.Create (1);

    NodeContainer master_node_container;
    master_node_container.Create (1);

    // crate 3 mapper container
    NodeContainer mapper_node_container;
    mapper_node_container.Create (3);

    YansWifiChannelHelper channel_helper = YansWifiChannelHelper::Default ();

    YansWifiPhyHelper phy_helper;
    phy_helper.SetChannel (channel_helper.Create ());

    WifiHelper wifi_helper;
    wifi_helper.SetRemoteStationManager ("ns3::AarfWifiManager");

    WifiMacHelper mac_helper;
    Ssid ssid = Ssid ("ns-3-ssid");
    mac_helper.SetType ("ns3::StaWifiMac",
                 "Ssid", SsidValue (ssid),
                 "ActiveProbing", BooleanValue (false));

    NetDeviceContainer client_ap_device_container;
    client_ap_device_container = wifi_helper.Install (phy_helper, mac_helper, client_node_container);

    mac_helper.SetType ("ns3::ApWifiMac", "Ssid", SsidValue (ssid));

    NetDeviceContainer client_device_container;
    client_device_container = wifi_helper.Install (phy_helper, mac_helper, client_node_container);

    mac_helper.SetType ("ns3::StaWifiMac","Ssid", SsidValue (ssid), "ActiveProbing", BooleanValue (false));

    NetDeviceContainer master_device_container;
    master_device_container = wifi_helper.Install (phy_helper, mac_helper, master_node_container);

    mac_helper.SetType ("ns3::StaWifiMac","Ssid", SsidValue (ssid), "ActiveProbing", BooleanValue (false));

    NetDeviceContainer mapper_device_container;
    mapper_device_container = wifi_helper.Install (phy_helper, mac_helper, mapper_node_container);

    mac_helper.SetType ("ns3::StaWifiMac","Ssid", SsidValue (ssid), "ActiveProbing", BooleanValue (false));

    Ptr<RateErrorModel> rate_error_model = CreateObject<RateErrorModel> ();
    rate_error_model->SetAttribute ("ErrorRate", DoubleValue (error));
    phy_helper.SetErrorRateModel("ns3::YansErrorRateModel");

    MobilityHelper mobility;

    mobility.SetPositionAllocator ("ns3::GridPositionAllocator",
                                   "MinX", DoubleValue (0.0),
                                   "MinY", DoubleValue (0.0),
                                   "DeltaX", DoubleValue (0.0),
                                   "DeltaY", DoubleValue (0.1),
                                   "GridWidth", UintegerValue (4),
                                   "LayoutType", StringValue ("RowFirst"));

    mobility.SetMobilityModel ("ns3::RandomWalk2dMobilityModel",
                               "Bounds", RectangleValue (Rectangle (-50, 50, -50, 50)));
    mobility.Install (client_node_container);

    mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
    mobility.Install (master_node_container);

    mobility.SetMobilityModel ("ns3::ConstantPositionMobilityModel");
    mobility.Install (mapper_node_container);

    InternetStackHelper stack;
    stack.Install (client_node_container);
    stack.Install (master_node_container);
    stack.Install (mapper_node_container);

    Ipv4AddressHelper address;
    Ipv4InterfaceContainer apNodeClientInterface = address.Assign(client_ap_device_container);

    unordered_map<int, Ipv4InterfaceContainer> interfaces;

    Ipv4InterfaceContainer staNodesMasterInterface = address.Assign(master_device_container);
    Ipv4InterfaceContainer staNodesMapperInterface = address.Assign(mapper_device_container);
    Ipv4InterfaceContainer staNodesClientInterface = address.Assign(client_device_container);

    address.SetBase ("10.1.3.0", "255.255.255.0");
    interfaces.insert({InterfacesNum::CLIENT, staNodesClientInterface});
    interfaces.insert({InterfacesNum::MASTER, staNodesMasterInterface});
    interfaces.insert({InterfacesNum::MAPPER, staNodesMapperInterface});

    Ipv4GlobalRoutingHelper::PopulateRoutingTables ();

    uint16_t port = GLOBAL_PORT;
    Ptr<client> clientApp = CreateObject<client> (port, interfaces);
    client_node_container.Get (0)->AddApplication (clientApp);
    clientApp->SetStartTime (Seconds (0.0));
    clientApp->SetStopTime (Seconds (duration));    

    Ptr<master> masterApp = CreateObject<master> (port, interfaces);
    master_node_container.Get (0)->AddApplication (masterApp);
    masterApp->SetStartTime (Seconds (0.0));
    masterApp->SetStopTime (Seconds (duration));

    // create 3 mapper application one for each node
    Ptr<mapper> mappersApp[MAPPERS_NUM];
    for (unsigned int i = 0; i < MAPPERS_NUM; i++)
    {
        mappersApp[i] = CreateObject<mapper> (port, interfaces, i);
        mapper_node_container.Get (i)->AddApplication (mappersApp[i]);
        mappersApp[i]->SetStartTime (Seconds (0.0));
        mappersApp[i]->SetStopTime (Seconds (duration)); 
    }

    NS_LOG_INFO ("Run Simulation");

    Ptr<FlowMonitor> flowMonitor;
    FlowMonitorHelper flowHelper;
    flowMonitor = flowHelper.InstallAll();

    monitor_data (&flowHelper, flowMonitor, error);

    Simulator::Stop (Seconds (duration));
    Simulator::Run ();

    return 0;
}
