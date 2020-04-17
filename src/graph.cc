#include "graph.hh"

#include <algorithm>
#include <iomanip>
#include <iostream>

#include "log.hh"
#include "utils.hh"

void Graph::addEdgePair(int src_id, int dst_id, int line_id, float duration,
                        float traffic)
{
    addEdge(src_id, dst_id, line_id, duration, traffic);
    addEdge(dst_id, src_id, line_id, duration, traffic);
}

void Graph::removeEdgePair(int src_id, int dst_id, int line_id)
{
    removeEdge(src_id, dst_id, line_id);
    removeEdge(dst_id, src_id, line_id);
}

void Graph::removeEdge(int src_id, int dst_id, int line_id)
{
    auto cond = [&dst_id, &line_id](const Edge& e) {
        return e.dst_id == dst_id && e.line_id == line_id;
    };

    adj_list[src_id].erase(
        std::remove_if(adj_list[src_id].begin(), adj_list[src_id].end(), cond),
        adj_list[src_id].end());
}

void Graph::addEdge(int src_id, int dst_id, int line_id, float duration,
                    float traffic)
{
    Edge edge = {src_id, dst_id, line_id, duration, traffic};

    if (edge.src_id >= order_)
        order_ = edge.src_id + 1;

    if (edge.dst_id >= order_)
        order_ = edge.dst_id + 1;

    adj_list.resize(order_);
    adj_list[edge.src_id].push_back(edge);
}

int Graph::getStationId(std::string name)
{
    for (int i = 0; i < order_; i++)
        if (!compareAdvanced(name, stations_data[i].name))
            return i;
    Log l(__FUNCTION__, true);
    l << "Can't find station " << name << "\n";
    return -1;
}

int Graph::getLineId(std::string type, std::string code)
{
    for (size_t i = 0; i < lines_data.size(); i++)
        if (!type.compare(lines_data[i].type)
            && !code.compare(lines_data[i].code))
            return i;
    return -1;
}

int Graph::getOrder()
{
    return order_;
}

float Graph::getTotalPopulation()
{
    float sum = 0;
    for (auto s : stations_data)
        sum += s.population;
    return sum;
}

float Graph::getTotalEmployment()
{
    float sum = 0;
    for (auto s : stations_data)
        sum += s.employment;
    return sum;
}

std::ostream& operator<<(std::ostream& os, Graph& G)
{
    for (int i = 0; i < G.order_; i++)
    {
        os << i << ":" << G.stations_data[i].name
           << "|p:" << G.stations_data[i].population
           << "|e:" << G.stations_data[i].employment << "|pos("
           << G.stations_data[i].position.latitude << ","
           << G.stations_data[i].position.longitude << ")"
           << G.stations_data[i].city << "(" << G.stations_data[i].city_id
           << "){";

        for (Edge e : G.adj_list[i])
            os << "(" << e.src_id << "->" << e.dst_id << "/l:" << e.line_id
               << "[" << G.lines_data[e.line_id].type << ","
               << G.lines_data[e.line_id].code << ","
               << G.lines_data[e.line_id].color << "]"
               << ",d:" << e.duration << ",t:" << e.traffic << ")";
        os << "}" << std::endl;
    }
    os << std::fixed << std::setprecision(2) << "order_:" << G.order_
       << ", stations_data.size():" << G.stations_data.size()
       << ", lines_data.size():" << G.lines_data.size()
       << ", pop:" << G.total_population_ << ", emp:" << G.total_employment_
       << "\n";

    return os;
}
