#include <algorithm>
#include <iostream>
#include <limits>
#include <map>
#include <set>
#include <utility>
#include <vector>

#include "graph.hh"

void Graph::browsePath(std::vector<std::pair<int, int>> best_parent, int src_id,
                       int dst_id, int i)
{
    if (best_parent[i].first == -1)
        return;
    browsePath(best_parent, src_id, dst_id, best_parent[i].first);

    std::vector<Edge> v = adj_list[best_parent[i].first];
    int dst = i;
    int line = best_parent[i].second;
    auto it = std::find_if(v.begin(), v.end(), [dst, line](const Edge& obj) {
        return obj.dst_id == dst && obj.line_id == line;
    });

    if (it != v.end())
    {
        int index = std::distance(v.begin(), it);
        float traffic;
        if (Graph::HPM)
        {
            traffic = station_list[src_id].population
                * (station_list[dst_id].employment / TOTAL_EMPLOYMENT);
        } else
        {
            traffic = station_list[src_id].employment
                * (station_list[dst_id].population / TOTAL_POPULATION);
        }

        adj_list[best_parent[i].first][index].traffic += traffic;
    } else
    {
        std::cerr << "Can't find the edge\n";
    }
}

void Graph::dijkstraDistribution()
{
    for (int i = 0; i < order_; i++)
        distribute_from(i);
}

void Graph::distribute_from(int src_id)
{
    std::set<std::pair<int, int>> setds;
    std::vector<int> dist(order_, std::numeric_limits<int>::max());
    std::vector<std::pair<int, int>> best_parent(order_);
    best_parent[src_id] = std::make_pair(-1, -1);

    setds.insert(std::make_pair(0, src_id));
    dist[src_id] = 0;

    while (!setds.empty())
    {
        std::pair<int, int> tmp = *(setds.begin());
        setds.erase(setds.begin());
        int u = tmp.second;

        for (auto adj : adj_list[u])
        {
            int v = adj.dst_id;
            int duration = adj.duration;

            if (dist[v] > dist[u] + duration)
            {
                if (dist[v] != std::numeric_limits<int>::max())
                    setds.erase(setds.find(std::make_pair(dist[v], v)));

                best_parent[v] = std::make_pair(u, adj.line_id);
                dist[v] = dist[u] + duration;
                setds.insert(std::make_pair(dist[v], v));
            }
        }
    }

    for (int dst_id = 0; dst_id < order_; dst_id++)
        browsePath(best_parent, src_id, dst_id, dst_id);
}
