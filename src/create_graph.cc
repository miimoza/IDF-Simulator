#include <climits>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <utility>

#include "graph.hh"
#include "log.hh"

using json = nlohmann::json;

static json stream_to_json(const std::string& filename)
{
    std::ifstream s(filename);

    json j;
    try
    {
        s >> j;
    } catch (std::exception& e)
    {
        Log l(__FUNCTION__, true);
        l << "Error Parsing Json: cannot load " << filename << "\n";
    }
    s.close();

    return j;
}

/*
std::shared_ptr<Station> get_station(station_vect v, const std::string& slug)
{
    for (auto s : v)
        if (!slug.compare(s->get_slug()))
            return s;
    return NULL;
}

static void fill_adj_station(station_vect v, std::shared_ptr<Station> s, json l,
                             const std::string& line_code, size_t i)
{
    std::cout << "station " << i << l.size() << l[i]["slug"] << "\n";
    std::string slug = l[i]["slug"];

    std::shared_ptr<Station> s1, s2;
    if (i < l.size() - 1)
    {
        log << "	avant " << i << l.size() << l[i + 1]["slug"] << "\n";
        s1 = get_station(v, l[i + 1]["slug"]);
        adjacency_station adj1 = std::make_pair(
            line_code, std::make_pair(s1, std::make_pair(0, 60)));
        s->add_adj(adj1);
    }

    if (i > 0)
    {
        log << "	apres " << i << l.size() << l[i - 1]["slug"] << "\n";
        s2 = get_station(v, l[i - 1]["slug"]);
        adjacency_station adj2 = std::make_pair(
            line_code, std::make_pair(s2, std::make_pair(0, 60)));
        s->add_adj(adj2);
    }
}

static void update_adj_lists(std::shared_ptr<Graph> g, const std::string& type)
{
    for (auto l : g->lines_["result"][type])
    {
        std::stringstream line_path;
        std::string code(l["code"]);
        code.erase(remove(code.begin(), code.end(), '\"'), code.end());
        line_path << "data/stations/" << type << "/" << code << ".json";
        json line = stream_to_json(line_path.str());
        l = line["result"]["stations"];
        for (size_t i = 0; i < l.size(); i++)
        {
            auto station = get_station(g->stations_data, l[i]["slug"]);
            std::stringstream line_id;
            line_id << type << code;
            if (!station)
                std::cerr << "error no station named " << l[i]["slug"] << "\n";
            else
                fill_adj_station(g->stations_data, station, l, line_id.str(),
i);
        }
    }
}*/

/*
static void update_id(std::shared_ptr<Graph> g)
{
    for (size_t i = 0; i < v.size(); i++)
        v[i]->set_id(i);
}*/

static float getLineDuration(std::string& type)
{
    if (!type.compare("metros"))
        return 1.5;
    if (!type.compare("rers"))
        return 4;
    if (!type.compare("tramways"))
        return 2;
    if (!type.compare("bus"))
        return 1.5;
    return 2;
}

static std::string& getLineColor(std::string& type, std::string& code)
{
    std::map<std::string, std::string> lines_color = {
        {"metros1", "#FFCD00"},    {"metros2", "#003CA6"},
        {"metros3", "#837902"},    {"metros3b", "#6EC4E8"},
        {"metros4", "#CF009E"},    {"metros5", "#FF7E2E"},
        {"metros6", "#6ECA97"},    {"metros7", "#FA9ABA"},
        {"metros7b", "#6ECA97"},   {"metros8", "#E19BDF"},
        {"metros9", "#B6BD00"},    {"metros10", "#C9910D"},
        {"metros11", "#704B1C"},   {"metros12", "#007852"},
        {"metros13", "#6EC4E8"},   {"metros14", "#62259D"},
        {"rersA", "#E2231A"},      {"rersB", "#7BA3DC"},
        {"rersC", "#C9910D"},      {"rersD", "#00AE41"},
        {"rersE", "#E37ED1"},      {"tramways1", "#003CA6"},
        {"tramways2", "#CF009E"},  {"tramways3a", "#FF7E2E"},
        {"tramways3b", "#00AE41"}, {"tramways4", "#E19BDF"},
        {"tramways5", "grey"},     {"tramways6", "grey"},
        {"tramways7", "#704B1C"},  {"tramways8", "#837902"}};

    std::stringstream s;
    s << type << code;
    return lines_color[s.str()];
}

int Graph::addLine(std::string type, std::string code, std::string color)
{
    lines_data.push_back({type, code, color});
    return lines_data.size() - 1;
}

int Graph::addStation(std::string name, float population, float employment)
{
    auto v = stations_data;
    auto it = std::find_if(v.begin(), v.end(), [&name](const StationData& s) {
        return s.name == name;
    });

    if (it != v.end())
    {
        Log l(__FUNCTION__, true);
        l << "La station " << name << " est deja presente dans stations_data\n";
        return std::distance(v.begin(), it);
    } else
    {
        int index = stations_data.size();
        stations_data.push_back({name, population, employment});
        return index;
    }
}

void Graph::initStations(json lines_json, std::string type)
{
    for (auto l : lines_json["result"][type])
    {
        std::stringstream line_path;
        std::string code(l["code"]);
        code.erase(remove(code.begin(), code.end(), '\"'), code.end());
        line_path << "data/stations/" << type << "/" << code << ".json";
        json line = stream_to_json(line_path.str());

        // Check if Json File is OK
        if (line.size() > 0)
        {
            auto stations = line["result"]["stations"];
            int line_id = addLine(type, code, getLineColor(type, code));

            int src_id = addStation(stations[0]["name"], 5, 5);
            for (size_t i = 0; i < stations.size() - 1; i++)
            {
                int dst_id = addStation(stations[i + 1]["name"], 5, 5);
                addEdgePair(src_id, dst_id, line_id, getLineDuration(type), 0);
                src_id = dst_id;
            }
        }
    }
}

void Graph::correctFailure()
{
    addEdgePair(getStationId("Magenta"), getStationId("Gare du Nord"), 0, 5, 0);
    // addEdgePair(getStationId("Anvers"), getStationId("Funiculaire Gare
    // basse"),
    //            0, 5, 0);
}

Graph::Graph(std::string folder_path)
{
    Log l("Graph");
    order_ = 0;
    std::stringstream lines_json_path;
    lines_json_path << folder_path << "/lines.json";
    json lines_json = stream_to_json(lines_json_path.str());

    l << "Import metros stations\n";
    initStations(lines_json, "metros");
    l << "Import rers stations\n";
    initStations(lines_json, "rers");
    // l << "Import rers tramways\n";
    // initStations(lines_json, "tramways");
    // l << "Import rers stations\n";
    // initStations(lines_json, "tramways");

    // update_adj_lists(g, "metros");
    // update_adj_lists(g, "rers");
    // update_adj_lists(g, "tramways");
    // update_adj_lists(g, "bus");

    // update_id(g->stations_data);
    correctFailure();
}
