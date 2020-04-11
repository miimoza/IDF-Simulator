#include "graph.hh"

#include <iostream>

#include "log.hh"

void Graph::addEdgePair(int src_id, int dst_id, int line_id, float duration,
                        float traffic)
{
    addEdge(src_id, dst_id, line_id, duration, traffic);
    addEdge(dst_id, src_id, line_id, duration, traffic);
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
        if (!name.compare(stations_data[i].name))
            return i;
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

void Graph::dump()
{
    for (int i = 0; i < order_; i++)
    {
        std::cout << i << ":" << stations_data[i].name
                  << "|p:" << stations_data[i].population
                  << "|e:" << stations_data[i].employment << "|pos("
                  << stations_data[i].position.latitude << ","
                  << stations_data[i].position.longitude << "){";

        for (Edge e : adj_list[i])
            std::cout << "(" << e.src_id << "->" << e.dst_id
                      << "/l:" << e.line_id << "[" << lines_data[e.line_id].type
                      << "," << lines_data[e.line_id].code << ","
                      << lines_data[e.line_id].color << "]"
                      << ",d:" << e.duration << ",t:" << e.traffic << ")";
        std::cout << "}" << std::endl;
    }
    std::cout << "order_:" << order_
              << ", stations_data.size():" << stations_data.size()
              << ", lines_data.size():" << lines_data.size() << "\n";
}

std::ostream& operator<<(std::ostream& os, const Graph& G)
{
    for (int i = 0; i < G.order_; i++)
    {
        os << i << ":" << G.stations_data[i].name
           << "|p:" << G.stations_data[i].population
           << "|e:" << G.stations_data[i].employment << "|pos("
           << G.stations_data[i].position.latitude << ","
           << G.stations_data[i].position.longitude << "){";

        for (Edge e : G.adj_list[i])
            os << "(" << e.src_id << "->" << e.dst_id << "/l:" << e.line_id
               << "[" << G.lines_data[e.line_id].type << ","
               << G.lines_data[e.line_id].code << ","
               << G.lines_data[e.line_id].color << "]"
               << ",d:" << e.duration << ",t:" << e.traffic << ")";
        os << "}" << std::endl;
    }
    os << "order_:" << G.order_
       << ", stations_data.size():" << G.stations_data.size()
       << ", lines_data.size():" << G.lines_data.size() << "\n";

    return os;
}
