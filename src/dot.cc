#include <fstream>
#include <iomanip>

#include "graph.hh"
#include "log.hh"

void Graph::generateDot(std::string filename)
{
    Log l(__FUNCTION__);
    l << "Generate " << filename << "\n";

    std::ofstream os;
    os.open(filename);
    os << "digraph AST {\n";
    os << "ratio=1; ranksep=2; nodesep=2; pin=true;\n";
    os << "node [shape=box,style=\"filled\"]\n";

    for (int i = 0; i < order_; i++)
    {
        std::string node_color;
        if (adj_list[i].size() > 2)
            node_color = "grey";
        else
            node_color = "white";

        os << i << " [fillcolor=\"" << node_color << "\", label=<<B>"
           << stations_data[i].name << "\n(pop:" << stations_data[i].population
           << ", emp:" << stations_data[i].employment << ", pos{"
           << stations_data[i].position.latitude << ","
           << stations_data[i].position.longitude << "})</B>>];\n";
    }

    for (int i = 0; i < order_; i++)
    {
        for (Edge edge : adj_list[i])
        {
            std::string edge_color = lines_data[edge.line_id].color;
            os << std::fixed << std::setprecision(2) << i << "->" << edge.dst_id
               << "[penwidth=" << 3 + (edge.traffic * 50 / total_population_)
               << ",color=\"" << edge_color << "\", label=\""
               << "d:" << edge.duration << " t:" << edge.traffic << "\"];\n";
        }
    }

    os << "}";
    os.close();
}
