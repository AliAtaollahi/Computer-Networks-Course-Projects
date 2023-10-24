#pragma once

#include <vector>
#include <string>

class Graph 
{
public:
    void run_command(std::vector<std::string> new_command);
    void build_graph(std::vector<std::string> new_command);
    void show();
    void modify_edge(std::vector<std::string> new_command);
    void delete_edge(std::vector<std::string> new_command);
    void execute_lsrp_algorithm(std::vector<std::string> new_command);
private:
    void print_dvrp(int node, int dis[MAX_NODES], int parent[MAX_NODES]);
    void execute_dvrp_algorithm(std::vector<std::string> new_command);
    void build_nxt_node(int node_inp);
    void print_shortest_path(std::vector<int>dis, std::vector<int> parent, int src);
    double execute_lsrp_on_node(int src);
    void print_iter_lsrp(int cnt, std::vector<int> &dis);
    int decode(int index, std::string edge);
    int adjacency[MAX_NODES][MAX_NODES];
    int nxt[MAX_NODES][MAX_NODES];
    int max_node_index;
};
