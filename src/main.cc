#include <fstream>
#include <iostream>

#include "graph.hh"
#include "log.hh"

int main(int argc, char* argv[])
{
    std::cout << "\033[0;33m================= IDF SIMULATOR V1.0 "
                 "=================\033[0m\n";

    (void)argc;
    (void)argv;

    std::ofstream logfile;
    logfile.open("graph.log");
    init_logs();
    Log::display = true;

    Graph G = Graph();
    logfile << G;
    G.dijkstraDistribution();
    logfile << G;
    G.generateDot("output.dot");

    logfile.close();
    return 0;
}
