#include <iostream>

#include "graph.hh"
#include "log.hh"

int main(int argc, char* argv[])
{
    std::cout << "==== IDF SIMULATOR V1.0 ====\n";

    (void)argc;
    (void)argv;

    init_logs();
    Log::display = true;

    Log l(__FUNCTION__);
    l << "\n";

    Graph G = Graph("data");

    G.dump();
    G.dijkstraDistribution();
    G.generateDot("output.dot");
    G.dump();

    return 0;
}
