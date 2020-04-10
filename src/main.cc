#include <iostream>

#include "graph.hh"

int main(int argc, char* argv[])
{
    std::cout << "==== IDF SIMULATOR V1.0 ====\n";

    (void)argc;
    (void)argv;

    std::vector<Edge> edges = {
        {0, 1, 1, 2, 0}, {1, 0, 1, 2, 0}, {1, 2, 1, 2, 0}, {2, 1, 1, 2, 0},
        {2, 3, 1, 2, 0}, {3, 2, 1, 2, 0}, {3, 4, 1, 2, 0}, {4, 3, 1, 2, 0},
        {5, 6, 2, 2, 0}, {6, 5, 2, 2, 0}, {6, 2, 2, 2, 0}, {2, 6, 2, 2, 0},
        {2, 7, 2, 2, 0}, {7, 2, 2, 2, 0}, {7, 8, 2, 2, 0}, {8, 7, 2, 2, 0},
        {9, 6, 3, 2, 0}, {6, 9, 3, 2, 0}, {6, 2, 3, 1, 0}, {2, 6, 3, 1, 0}};

    // Graph G = Graph();

    // G.dump();

    // for (auto e : edges)
    //    G.addEdge(e);
    Graph G(edges, 10);

    Edge e = {10, 2, 4, 2, 0};
    G.addEdge(e);

    G.dump();

    std::cout << "Distribution...\n";
    G.dijkstraDistribution();

    std::cout << "Generate Dot...\n";
    G.generateDot("output.dot");

    return 0;
}
