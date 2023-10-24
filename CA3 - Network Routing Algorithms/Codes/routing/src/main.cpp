#include <bits/stdc++.h>
#include <chrono>
#include "manual.hpp"
#include "graph.hpp"

using namespace std::chrono;
using namespace std;

bool get_line_as_vector(vector<string> &result, string delimiters = " ");

int main()
{
    Graph topology_network;
    vector<string> line;
    while (get_line_as_vector(line))
        topology_network.run_command(line);
}

bool get_line_as_vector(vector<string> &result, string delimiters)
{
    result.clear();
    string line, temp;
    if (!getline(cin, line))
        return 0;
    for (char i : line)
        if (delimiters.find(i) != string::npos)
        {
            if (!temp.empty())
                result.push_back(temp);
            temp = "";
        }
        else
            temp += i;
    if (!temp.empty())
        result.push_back(temp);
    return 1;
}
