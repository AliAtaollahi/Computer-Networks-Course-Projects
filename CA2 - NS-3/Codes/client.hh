#include "ns3-includes.hh"
#include "paket-sender.hh"

#include <vector>

using namespace ns3;
using namespace std;

class client : public Application
{
public:
    client (uint16_t port, unordered_map<int, Ipv4InterfaceContainer>& interfaces_);
    virtual ~client ();

private:
    virtual void StartApplication (void);
    void HandleRead (Ptr<Socket> socket);


    unordered_map<int, Ipv4InterfaceContainer> interfaces;

    uint16_t port;
    Ptr<Socket> socket;
    vector<Ptr<Socket>> mappers_sockets;
    vector<InetSocketAddress> mappers_sockets_address;

    Ptr<Socket> mapper_0_socket;
    Ptr<Socket> mapper_1_socket;
    Ptr<Socket> mapper_2_socket;

};

client::client (uint16_t port, unordered_map<int, Ipv4InterfaceContainer>& interfaces_)
        : interfaces(interfaces_),
          port (port),
          mappers_sockets({}),
          mappers_sockets_address({})
{
    std::srand (time(0));
}
    
client::~client ()
{
}

void client::StartApplication (void)
{
    // connect to  master
    Ptr<Socket> sock = Socket::CreateSocket (GetNode (), UdpSocketFactory::GetTypeId ());
    InetSocketAddress sockAddr (interfaces[InterfacesNum::MASTER].GetAddress(0), port);
    sock->Connect (sockAddr);


    // bind to mapper_0
    for (int i = 0; i < MAPPERS_NUM; i++)
    {
        mappers_sockets.push_back(Socket::CreateSocket (GetNode (), UdpSocketFactory::GetTypeId ()));
        mappers_sockets_address.push_back(InetSocketAddress (interfaces[InterfacesNum::CLIENT].GetAddress(0), MAPPERS_PORT_INIT + i));
        if (mappers_sockets[i]->Bind (mappers_sockets_address[i]) == -1)
        {
            cout << KRED << "ERROR_BINDING_CLIENT_TO_MAPPER" << KWHT << endl;
            exit(EXIT_FAILURE);
        }
        mappers_sockets[i]->SetRecvCallback (MakeCallback (&client::HandleRead, this));
    }

    generate_initial_traffic(sock, 0, "(UDP)client->master");
}

void client::HandleRead (Ptr<Socket> socket)
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

        cout << KNRM << "\t\t\t CLIENT CATCHED : " << static_cast<char>(buffer) << KWHT << endl << endl;
    }
}