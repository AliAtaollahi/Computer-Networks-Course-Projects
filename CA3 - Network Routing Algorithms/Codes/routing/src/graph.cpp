#include <vector>
#include <string>
#include <iostream>
#include <chrono>
#include <algorithm>
#include <queue>
#include <set>
#include <map>

#include "manual.hpp"
#include "graph.hpp"
#include "table.hpp"

using namespace std;
using namespace std::chrono;

void Graph::build_nxt_node(int node_inp)
{
    int lst = max_node_index + 1;
    for (int i = max_node_index; i >= 1; i--)
    {
        nxt[node_inp][i] = lst;
        if (adjacency[node_inp][i] != -1)
            lst = i;
    }
}

void Graph::modify_edge(vector<string> new_command)
{
    int node1 = decode(0, new_command[1]);
    int node2 = decode(1, new_command[1]);
    if (node1 > max_node_index || node2 > max_node_index)
    {
        cout << "Error: Node index out of range" << endl;
        return;
    }
    else if (node1 == node2)
    {
        cout << "Error: Edge endpoints must be diffrent" << endl;
        return;
    }
    max_node_index = max(node1, max_node_index);
    max_node_index = max(node2, max_node_index);
    int weight = decode(2, new_command[1]);
    build_nxt_node(node1);
    build_nxt_node(node2);
    if (node1 == node2)
    {
        cout << "Error: Edge endpoints must be diffrent" << endl;
        return;
    }
    adjacency[node1][node2] = weight;
    adjacency[node2][node1] = weight;
    cout << "Edge modified" << endl;
}

void Graph::execute_lsrp_algorithm(vector<string> new_command)
{
    double time_taken = 0;
    if (new_command.size() == 2)
    {
        int node = decode(0, new_command[1]);
        time_taken = execute_lsrp_on_node(node);
    }
    else
    {
        for (int i = 1; i <= max_node_index; i++)
        {
            double time_node = execute_lsrp_on_node(i);
            time_taken += time_node;
            cout << "Time taken by lsrp node i: "
                 << time_node << " nanoseconds" << endl;
        }
    }
    cout << "Time taken by lsrp: "
         << time_taken << " nanoseconds" << endl;
}

void Graph::print_iter_lsrp(int cnt, vector<int> &dis)
{
    TextTable t('-', '|', '-');
    t.add(" Dest ");
    for (int i = 1; i <= max_node_index; i++)
        t.add(to_string(i) + "  ");
    t.endOfRow();
    t.add(" Cost ");
    for (int i = 1; i <= max_node_index; i++)
        t.add(to_string(dis[i]));
    t.endOfRow();
    cout << "Iter " << cnt << ":" << endl;
    cout << t << endl;
}

void Graph::print_shortest_path(vector<int> dis, vector<int> parent, int src)
{
    cout << "Shortest path from node " << src << ":" << endl;
    TextTable t('.', '|', '.');
    t.add(" Dest ");
    for (int i = 1; i <= max_node_index; i++)
        t.add(to_string(i) + "  ");
    t.endOfRow();
    t.add(" Cost ");
    for (int i = 1; i <= max_node_index; i++)
        t.add(to_string(dis[i]));
    t.endOfRow();
    t.add(" Path ");
    for (int i = 1; i <= max_node_index; i++)
    {
        if (i == src)
        {
            t.add(" - ");
            continue;
        }
        vector<int> path;
        int now = i;
        while (now != -1)
        {
            path.push_back(now);
            now = parent[now];
        }
        reverse(path.begin(), path.end());
        string path_str = "";
        for (int j = 0; j < path.size(); j++)
        {
            path_str += to_string(path[j]);
            if (j != path.size() - 1)
                path_str += "->";
        }
        t.add(path_str);
    }
    t.endOfRow();
    cout << t << endl;
}

double Graph::execute_lsrp_on_node(int src)
{
    auto start = high_resolution_clock::now();
    vector<int> mark, dis, parent;
    for (int i = 1; i <= max_node_index + 1; i++)
    {
        mark.push_back(0);
        dis.push_back(-1);
        parent.push_back(-1);
    }
    set<pair<int, int>> dijkstra_set;
    dijkstra_set.insert(make_pair(0, src));
    dis[src] = 0;
    int cnt = 0;
    while (dijkstra_set.size())
    {
        cnt++;
        pair<int, int> next_node = *dijkstra_set.begin();
        dijkstra_set.erase(next_node);
        int now_vertex = next_node.second;
        for (int i = 1; i <= max_node_index; i++)
        {
            int w_edge = adjacency[now_vertex][i];
            if (w_edge == -1)
                continue;
            if (dis[now_vertex] + w_edge < dis[i] || dis[i] == -1)
            {
                dijkstra_set.erase(make_pair(dis[i], i));
                dis[i] = dis[now_vertex] + w_edge;
                parent[i] = now_vertex;
                dijkstra_set.insert(make_pair(dis[i], i));
            }
            i = nxt[now_vertex][i] - 1;
        }
        if (dijkstra_set.size())
            print_iter_lsrp(cnt, dis);
    }
    print_shortest_path(dis, parent, src);
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(stop - start);
    return duration.count();
}

int Graph::decode(int index, string edge)
{
    int cnt = 0;
    int value = 0;
    for (int i = 0; i < edge.size(); i++)
    {
        if (edge[i] == '-')
        {
            if (cnt == index)
                return value;
            value = 0;
            cnt++;
        }
        else
        {
            value *= 10;
            value += (edge[i] - '0');
        }
    }
    return value;
}

void Graph::build_graph(vector<string> new_command)
{
    max_node_index = 0;
    for (int i = 1; i <= 500; i++)
    {
        for (int j = 1; j <= 500; j++)
        {
            adjacency[i][j] = -1;
        }
        adjacency[i][i] = 0;
    }
    for (int i = 1; i < new_command.size(); i++)
    {
        int node1 = decode(0, new_command[i]);
        int node2 = decode(1, new_command[i]);
        if (node1 == node2)
        {
            cout << "Error: Self loop is not allowed!" << endl;
            return;
        }
        max_node_index = max(node1, max_node_index);
        max_node_index = max(node2, max_node_index);
        int weight = decode(2, new_command[i]);
        adjacency[node1][node2] = weight;
        adjacency[node2][node1] = weight;
    }
    for (int i = 1; i <= max_node_index; i++)
        build_nxt_node(i);
    cout << "Graph is built successfully!" << endl;
}

void Graph::show()
{
    TextTable t('.', '|', '.');
    t.add("");
    for (int i = 1; i <= max_node_index; i++)
        t.add("(" + to_string(i) + ")");
    t.endOfRow();
    for (int i = 1; i <= max_node_index; i++)
    {
        t.add("(" + to_string(i) + ")");
        for (int j = 1; j <= max_node_index; j++)
            t.add(to_string(adjacency[i][j]));
        t.endOfRow();
    }
    t.setAlignment(3, TextTable::Alignment::LEFT);
    cout << t << endl;
}

void Graph::delete_edge(vector<string> new_command)
{
    int node1 = decode(0, new_command[1]);
    int node2 = decode(1, new_command[1]);
    if (node1 > max_node_index || node2 > max_node_index)
    {
        cout << "Error: Node does not exist!" << endl;
        return;
    }
    else if (node1 == node2)
    {
        cout << "Error: Self loop is not allowed!" << endl;
        return;
    }
    else if (adjacency[node1][node2] == -1)
    {
        cout << "Error: Edge does not exist!" << endl;
        return;
    }
    max_node_index = max(node1, max_node_index);
    max_node_index = max(node2, max_node_index);
    if (node1 != node2)
    {
        adjacency[node1][node2] = -1;
        adjacency[node2][node1] = -1;
    }
    build_nxt_node(node1);
    build_nxt_node(node2);
    cout << "Edge is deleted successfully!" << endl;
}

void Graph::print_dvrp(int node, int dis[MAX_NODES], int parent[MAX_NODES])
{
    TextTable t('.', '|', '.');
    t.add(" Dest ");
    t.add(" Next Hop ");
    t.add(" Dist ");
    t.add(" Shortest Path ");
    t.endOfRow();

    for (int i = 1; i <= max_node_index; i++)
    {
        int next_hop = parent[i];
        if (parent[i] == node || parent[i] == -1)
            next_hop = i;
        t.add(to_string(i));
        t.add(to_string(next_hop));
        t.add(to_string(dis[i]));
        if (dis[i] == -1)
        {
            t.add("[]");
            t.endOfRow();
            continue;
        }
        vector<int> path;
        int now = i;
        while (now != -1)
        {
            path.push_back(now);
            now = parent[now];
        }
        reverse(path.begin(), path.end());
        string final_path = " [";
        for (int j = 0; j < path.size(); j++)
        {
            final_path += to_string(path[j]);
            if (j != (int)path.size() - 1)
                final_path += " -> ";
        }
        final_path += "] ";
        t.add(final_path);
        t.endOfRow();
    }
    cout << t << endl;
}

void Graph::execute_dvrp_algorithm(vector<string> new_command)
{
    auto start = high_resolution_clock::now();
    int dis[MAX_NODES][MAX_NODES], parent[MAX_NODES][MAX_NODES];
    for (int i = 1; i <= max_node_index; i++)
    {
        for (int j = 1; j <= max_node_index; j++)
        {
            dis[i][j] = -1;
            parent[i][j] = -1;
            if (adjacency[i][j] != -1)
            {
                dis[i][j] = adjacency[i][j];
                parent[i][j] = i;
            }
        }
        dis[i][i] = 0;
        parent[i][i] = -1;
    }
    for (int i = 1; i <= max_node_index + 1; i++)
    {
        for (int j = 1; j <= max_node_index; j++)
        {
            for (int k = 1; k <= max_node_index; k++)
            {
                for (int l = 1; l <= max_node_index; l++)
                {
                    int w_edge = adjacency[j][k];
                    if (dis[l][j] == -1)
                        continue;
                    if (w_edge == -1)
                        continue;
                    if (dis[l][k] == -1 || dis[l][j] + w_edge < dis[l][k])
                    {
                        dis[l][k] = dis[l][j] + w_edge;
                        parent[l][k] = j;
                    }
                }
                k = nxt[j][k] - 1;
            }
        }
    }
    if (new_command.size() == 2)
    {
        int node = decode(0, new_command[1]);
        print_dvrp(node, dis[node], parent[node]);
    }
    else
    {
        for (int i = 1; i <= max_node_index; i++)
        {
            print_dvrp(i, dis[i], parent[i]);
        }
    }
    auto stop = high_resolution_clock::now();
    auto duration = duration_cast<nanoseconds>(stop - start);
    cout << "Time taken by dvrp: "
         << duration.count() << " nanoseconds" << endl;
}

void Graph::run_command(vector<string> new_command)
{
    string command_type = new_command[0];
    if (command_type == TOPOLOGY_COMMAND)
        this->build_graph(new_command);
    else if (command_type == SHOW_COMMAND)
        this->show();
    else if (command_type == MODIFY_COMMAND)
        this->modify_edge(new_command);
    else if (command_type == DELETE_COMMAND)
        this->delete_edge(new_command);
    else if (command_type == LSRP_COMMAND)
        this->execute_lsrp_algorithm(new_command);
    else if (command_type == DVRP_COMMAND)
        this->execute_dvrp_algorithm(new_command);
}
