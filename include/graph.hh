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
    std::string city;
    int city_id;
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

    // VALUE
    float total_population_;
    float total_employment_;

    // GRAPH CREATION
    Graph();
    void initStations(CSV lines_csv, std::string type);
    int addStation(std::string name, float population, float employment,
                   Pos position, std::string city, int city_id);
    int addLine(std::string type, std::string code, std::string color);

    void addStationsPopulation();
    void addStationsEmployment();
    void addStationsPosition();
    void addStationCity();
    void sharePopEmp();
    void correctFailure();

    // CASUAL OPERATIONS
    void addEdgePair(int src_id, int dst_id, int line_id, float duration,
                     float traffic);
    void addEdge(int src_id, int dst_id, int line_id, float duration,
                 float traffic);
    void removeEdgePair(int src_id, int dst_id, int line_id);
    void removeEdge(int src_id, int dst_id, int line_id);
    int getStationId(std::string name);
    int getLineId(std::string type, std::string code);
    int getOrder();
    float getTotalPopulation();
    float getTotalEmployment();

    // DIJKSTRA DISTRIBUTION
    void dijkstraDistribution();
    void distributeFrom(int src_id);
    void browsePath(std::vector<std::pair<int, int>> best_parent, int src_id,
                    int dst_id, int i);

    // GRAPHVIZ GENERATION
    void generateDot(std::string filename);

    // DEBUG
    friend std::ostream& operator<<(std::ostream& os, Graph& G);

private:
    int order_;
};
