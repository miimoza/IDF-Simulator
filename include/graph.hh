#include <map>
#include <string>
#include <vector>

#define TOTAL_POPULATION 20
#define TOTAL_EMPLOYMENT 20

struct Station
{
    std::string name;
    float population;
    float employment;
};

struct Edge
{
    int src_id, dst_id, line_id, duration;
    float traffic;
};

class Graph
{
public:
    std::vector<std::vector<Edge>> adj_list;
    bool HPM = true;
    std::map<int, Station> station_list = {
        {0, {"Charles de Gaulle Etoile", 2, 3}},
        {1, {"Auber", 1, 5}},
        {2, {"Chatelet", 1, 2}},
        {3, {"Gare de Lyon", 1, 2}},
        {4, {"Nation", 3, 1}},
        {5, {"La Plaine Saint-Denis", 4, 1}},
        {6, {"Gare du Nord", 2, 3}},
        {7, {"Saint-Michel", 1, 1}},
        {8, {"Denfert-Rocherau", 2, 1}},
        {9, {"Stade de France Saint-Denis", 3, 1}},
        {10, {"Juvisy", 10, 1}}};

    std::map<int, std::string> line_colors = {{1, "#E2231A"},
                                              {2, "#7BA3DC"},
                                              {3, "#00AE41"},
                                              {4, "#00AE41"}};

    Graph(std::vector<Edge> const& edges, int order);
    Graph();

    void addEdge(Edge& e);
    int getOrder();
    void dump();

    void generateDot(std::string filename);

    void dijkstraDistribution();
    void distribute_from(int src_id);
    void browsePath(std::vector<std::pair<int, int>> best_parent, int src_id,
                    int dst_id, int i);

private:
    int order_;
};
