#pragma once

#include <string>
#include <vector>

#define TOTAL_POPULATION 1265
#define TOTAL_EMPLOYMENT 1265

using CSV = std::vector<std::vector<std::string>>;

struct Pos
{
    float latitude;
    float longitude;
};

struct StationData
{
    std::string name;
    float population;
    float employment;
    Pos position;
};

struct LineData
{
    std::string type;
    std::string code;
    std::string color;
};

struct Edge
{
    int src_id, dst_id, line_id;
    float duration, traffic;
};

class Graph
{
public:
    std::vector<std::vector<Edge>> adj_list;

    // OPTIONS
    bool HPM = true;

    // DATA
    std::vector<StationData> stations_data;
    std::vector<LineData> lines_data;

    // GRAPH CREATION
    Graph();
    void initStations(CSV lines_csv, std::string type);
    int addStation(std::string name, float population, float employment,
                   Pos position);
    int addLine(std::string type, std::string code, std::string color);
    void addStationsPosition();
    void correctFailure();

    // CASUAL OPERATIONS
    void addEdgePair(int src_id, int dst_id, int line_id, float duration,
                     float traffic);
    void addEdge(int src_id, int dst_id, int line_id, float duration,
                 float traffic);
    int getStationId(std::string name);
    int getLineId(std::string type, std::string code);
    int getOrder();
    void dump();

    // DIJKSTRA DISTRIBUTION
    void dijkstraDistribution();
    void distributeFrom(int src_id);
    void browsePath(std::vector<std::pair<int, int>> best_parent, int src_id,
                    int dst_id, int i);

    // GRAPHVIZ GENERATION
    void generateDot(std::string filename);

    // DEBUG
    friend std::ostream& operator<<(std::ostream& os, const Graph& G);

private:
    int order_;
};
