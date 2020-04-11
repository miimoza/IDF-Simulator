#include <fstream>

#include "graph.hh"
#include "log.hh"

void Graph::generateDot(std::string filename)
{
    Log l(__FUNCTION__);
    l << "Generate " << filename << "\n";

    std::ofstream os;
    os.open(filename);
    os << "digraph AST {ranksep=2;nodesep=2;\n";

    for (int i = 0; i < order_; i++)
    {
        std::string node_color;
        if (adj_list[i].size() > 2)
            node_color = "grey";
        else
            node_color = "white";

        os << i << " [shape=box,style=\"filled\",pos=\"2,3\",fillcolor=\""
           << node_color << "\",label=<<B>" << stations_data[i].name
           << "\n(pop:" << stations_data[i].population
           << ", emp:" << stations_data[i].employment << ")</B>>];\n";

        for (Edge edge : adj_list[i])
        {
            std::string edge_color = lines_data[edge.line_id].color;
            os << i << "->" << edge.dst_id << "[penwidth=" << edge.traffic / 5
               << ",color=\"" << edge_color << "\", label=\""
               << "d:" << edge.duration << " t:" << edge.traffic << "\"];\n";
        }
    }

    os << "}";
}
