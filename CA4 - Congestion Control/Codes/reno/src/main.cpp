#include <chrono>
#include <iostream>
#include <cmath>
#include <climits>
#include <vector>

using namespace std;

#define FAST_START true

enum CongestionMethod 
{
    slow_start,
    congestion_avoidance,
    fast_retransmit,
    aimd
};

struct Packet
{
    int seq_num;
    int ack_num;
    double rtt;
    bool is_lost;
};

class TCP_Reno 
{
private:
    size_t cwnd;
    size_t ssthresh;
    int dup_ack_count;
    double rtt;

    bool time_out() 
    {
        return false;
    }

public:
    TCP_Reno() 
    {
        cwnd = 1;
        ssthresh = INT_MAX;
        rtt = 100;
        dup_ack_count = 0;
    }

    void handle_ack_packet(Packet& packet)
    {
        if (!packet.is_lost)
        {
            dup_ack_count = 0;
            rtt = (rtt + packet.rtt) / 2;
            if (cwnd < ssthresh) 
            {
                cwnd = cwnd * 2;
            } 
            else 
            {
                cwnd += 1;
            }
        } 
        else // packet lost
        {
            dup_ack_count += 1;
            if (dup_ack_count == 2 || time_out()) 
            {
                ssthresh = cwnd / 2 ;
                if (!FAST_START)
                {
                    cwnd = 1;
                }
                else
                {
                    cwnd = ssthresh + 3;
                }
            }
            else
            {
                if (cwnd < ssthresh) 
                {
                    cwnd = cwnd * 2;
                } 
                else 
                {
                    cwnd += 1;
                }
            }
        }
    }

    size_t get_cwnd() 
    {
        return cwnd;
    }

    size_t get_ssthresh() 
    {
        return ssthresh;
    }
};

vector<Packet> create_packets(int num_packets, double packet_loss_rate) 
{
    vector<Packet> packets;
    for (int i = 0; i < num_packets; i++) 
    {
        Packet packet;
        packet.seq_num = i;
        packet.ack_num = -1;
        packet.is_lost = false;
        packets.push_back(packet);
    }
    // packet loss
    for (int i = 1; i < num_packets; i++) 
    {
        double rand_num = (double) rand() / RAND_MAX;
        if (rand_num < packet_loss_rate)
            packets[i].is_lost = true;
    }

    // rtt a random number between 100 and 150
    for (int i = 0; i < num_packets; i++) 
        packets[i].rtt = 100 + (double) rand() / RAND_MAX * 50;

    packets[9].is_lost = true;
    packets[2].is_lost = true;
    
    return packets;
}

class TCP_Reno_Simulator
{
public:
    TCP_Reno_Simulator() {}
    void simulate(int num_packets, double packet_loss_rate, string output_file_name)
    {
        // open file as output
        FILE* output_file = fopen(output_file_name.c_str(), "w");

        // create packets
        vector<Packet> packets = create_packets(num_packets, packet_loss_rate);

        TCP_Reno tcp_reno;

        int last_successfullly_sent_packet = 1;    
        for (int i = 0; i < num_packets; i++) 
        {
            // send packet
            Packet packet = packets[i];
            if (!packet.is_lost) 
                last_successfullly_sent_packet = i + 2;

            packet.ack_num = last_successfullly_sent_packet;

            tcp_reno.handle_ack_packet(packet);
            cout << "cwnd: " << tcp_reno.get_cwnd() << endl;
            cout << "ssthresh: " << tcp_reno.get_ssthresh() << endl;
            cout << "ssthresh: " << packet.is_lost << endl << endl;

            // write in a result.txt file
            fprintf(output_file, "%zu\n",tcp_reno.get_cwnd());

        }
        fprintf(output_file, "%zu",tcp_reno.get_cwnd());
        fclose(output_file);
    }
};


int main() 
{
    auto start = chrono::high_resolution_clock::now();
    srand(time(NULL));
    TCP_Reno_Simulator trs = TCP_Reno_Simulator();
    trs.simulate(200, 0.3, "result1.txt");
    trs.simulate(200, 0.5, "result2.txt");
    trs.simulate(200, 0.7, "result3.txt");
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
 
    cout << "Time taken by function: "
         << duration.count() << " microseconds" << endl;

    return 0;
}