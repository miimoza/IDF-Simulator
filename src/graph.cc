#include "graph.hh"

#include <fstream>
#include <iostream>
#include <map>

Graph::Graph()
{
    order_ = 0;
}

Graph::Graph(std::vector<Edge> const& edges, int order)
    : order_(order)
{
    adj_list.resize(order);
    for (auto& edge : edges)
        adj_list[edge.src_id].push_back(edge);
}

void Graph::addEdge(Edge& e)
{
    order_ += 1;
    adj_list.resize(order_);
    adj_list[e.src_id].push_back(e);
}

int Graph::getOrder()
{
    return order_;
}

void Graph::dump()
{
    for (int i = 0; i < order_; i++)
    {
        for (Edge e : adj_list[i])
            std::cout << "(" << i << "->" << e.dst_id << ", l:" << e.line_id
                      << ", d:" << e.duration << ", t:" << e.traffic << ") ";
        std::cout << std::endl;
    }
}

void Graph::generateDot(std::string filename)
{
    std::cout << "generate " << filename << "...\n";

    std::ofstream os;
    os.open(filename);
    os << "digraph AST {\nranksep=2;\nnodesep=2;\n";

    for (int i = 0; i < order_; i++)
    {
        for (Edge edge : adj_list[i])
        {
            std::string edge_color = line_colors[edge.line_id];
            os << i << "->" << edge.dst_id
               << "[penwidth=" << 3 + (edge.traffic * 50 / TOTAL_POPULATION)
               << ",color=\"" << edge_color << "\", label=\""
               << "d:" << edge.duration << " t:" << edge.traffic << "\"];\n";

            if (adj_list[i].size() > 2)
                edge_color = "grey";

            os << i << " [shape=box,style=\"filled\",fillcolor=\"" << edge_color
               << "\",label=<<B>" << station_list[i].name
               << "\n(pop:" << station_list[i].population
               << ", emp:" << station_list[i].employment << ")</B>>];\n";
        }
    }

    os << "}";
}
