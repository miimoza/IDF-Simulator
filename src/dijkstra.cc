#include <algorithm>
#include <iostream>
#include <limits>
#include <set>
#include <utility>
#include <vector>

#include "graph.hh"
#include "log.hh"

void Graph::browsePath(std::vector<std::pair<int, int>> best_parent, int src_id,
                       int dst_id, int i)
{
    if (best_parent[i].first == -1)
        return;
    browsePath(best_parent, src_id, dst_id, best_parent[i].first);

    std::vector<Edge> v = adj_list[best_parent[i].first];
    int dst = i;
    int line = best_parent[i].second;
    auto it = std::find_if(v.begin(), v.end(), [dst, line](const Edge& e) {
        return e.dst_id == dst && e.line_id == line;
    });

    if (it != v.end())
    {
        int index = std::distance(v.begin(), it);
        float traffic;
        if (Graph::HPM)
        {
            traffic = stations_data[src_id].population
                * (stations_data[dst_id].employment / TOTAL_EMPLOYMENT);
        } else
        {
            traffic = stations_data[src_id].employment
                * (stations_data[dst_id].population / TOTAL_POPULATION);
        }

        adj_list[best_parent[i].first][index].traffic += traffic;
    } else
    {
        Log l(__FUNCTION__, true);
        l << "Can't find the edge (dst:" << stations_data[dst].name << "/"
          << lines_data[line].type << lines_data[line].code << ")\n";
    }
}

void Graph::dijkstraDistribution()
{
    Log l(__FUNCTION__);
    for (int i = 0; i < order_; i++)
    {
        l << "Distribute from " << stations_data[i].name << "(" << i << "/"
          << order_ << ")\n";
        distributeFrom(i);
    }
}

void Graph::distributeFrom(int src_id)
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
