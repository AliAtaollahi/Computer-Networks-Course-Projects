#include <chrono>
#include <cmath>
#include <algorithm>
#include <fstream>
#include <iostream>
#include <climits>
#include <vector>

#define FAST_START false

using namespace std;

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

class TCP_BBR {
public:
    TCP_BBR()
    {
        cwnd = 80;
        ssthresh = INT_MAX;
        rtt_ = 1;
        min_rtt = 0;
        round_start_time = chrono::steady_clock::now();
        next_send_time = round_start_time + chrono::microseconds(
              static_cast<int>(cwnd * 1000000.0 / rtt_));
    }

    void packet_sent() {
        if (chrono::steady_clock::now() >= next_send_time) 
        {
            // Time to send a new packet
            next_send_time = chrono::steady_clock::now() +
                chrono::microseconds(static_cast<int>(
                    cwnd * 1000000.0 / rtt_));
        }
    }

    void handle_ack_packet(bool is_lost)
    {
        if(!is_lost)
        {
            auto now = chrono::steady_clock::now();
            float rtt = chrono::duration_cast<chrono::microseconds>(
                now - round_start_time).count() / 1000.0;

            if (min_rtt == 0) 
            {
                min_rtt = rtt;
            } 
            else 
            {
                min_rtt = min(min_rtt, rtt);
            }

            float bytes_acked = cwnd * rtt / min_rtt;
            float k = pow(cwnd, 3.0 / 4.0) /
                sqrt(bytes_acked * 0.9 + cwnd * cwnd * 0.1);
            cwnd += k;
            cwnd = min(cwnd, ssthresh);

            round_start_time = now;
        }
        else
        {
            cwnd *= 0.5;
        }
    }

    float get_cwnd()
    {
        return cwnd;
    }

    float get_ssthresh()
    {
        return ssthresh;
    }

private:
    float cwnd; // congestion window
    float ssthresh;
    float rtt_;
    float min_rtt;

    chrono::time_point<chrono::steady_clock> round_start_time;
    chrono::time_point<chrono::steady_clock> next_send_time;
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

class TCP_BBR_Simulator
{
public:
    TCP_BBR_Simulator() {}
    void simulate(int num_packets, float packet_loss_rate, string output_file_name)
    {
        // open file as output
        ofstream my_file(output_file_name);

        // create packets
        vector<Packet> packets = create_packets(num_packets, packet_loss_rate);

        TCP_BBR tcp_bbr;

        // send packets
        for (int i = 0; i < num_packets; ++i) {
            tcp_bbr.packet_sent();
            Packet packet = packets[i];
            cout << packet.is_lost << endl;
            tcp_bbr.handle_ack_packet(packet.is_lost);
            // simulate some packets being lost
            // if (i % 10 == 0) 
            // {
            //     tcp_bbr.handle_ack_packet(true);
            // }
            // else 
            // {
            //     tcp_bbr.handle_ack_packet(false);
            // }
            // cout << tcp_bbr.get_cwnd() << endl;
            // cout << tcp_bbr.get_ssthresh() << endl;
            my_file << static_cast<int>(tcp_bbr.get_cwnd()) << endl;
        }
        my_file << static_cast<int>(tcp_bbr.get_cwnd());
        my_file.close();

    }
};


int main() {
    auto start = chrono::high_resolution_clock::now();
    srand(time(NULL));
    TCP_BBR_Simulator tbs = TCP_BBR_Simulator();
    tbs.simulate(200, 0.1, "result1.txt");
    tbs.simulate(200, 0.1, "result2.txt");
    tbs.simulate(200, 0.1, "result3.txt");
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
 
    cout << "Time taken by function: "
         << duration.count() << " microseconds" << endl;

    return 0;
}
