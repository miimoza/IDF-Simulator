#include <boost/algorithm/string.hpp>
#include <climits>
#include <exception>
#include <fstream>
#include <iostream>
#include <map>
#include <memory>
#include <sstream>
#include <string>
#include <utility>

#include "csv_parser.hh"
#include "graph.hh"
#include "log.hh"
#include "utils.hh"

static float getTypeId(std::string& type)
{
    if (!type.compare("metros"))
        return 0;
    if (!type.compare("rers"))
        return 1;
    if (!type.compare("tramways"))
        return 2;
    if (!type.compare("bus"))
        return 3;
    return 0;
}

static float getTypeDuration(std::string& type)
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

void Graph::addStationsPosition()
{
    Log l(__FUNCTION__);
    CSVParser csv_parser("data/stations_position.csv", ";");
    CSV stations_position = csv_parser.getData();

    // std::cout << stations_position[i][0] << "/" <<
    // stations_position[i][7]
    //          << "/" << stations_position[i][9] << "\n";

    size_t n = 0;
    for (size_t i = 0; i < stations_position.size(); i++)
    {
        for (size_t j = 0; j < stations_data.size(); j++)
        {
            std::string s1 = boost::to_lower_copy(stations_position[i][6]);
            std::string s2 = boost::to_lower_copy(stations_data[j].name);
            if (!s1.compare(s2) && !stations_data[j].position.latitude)
            {
                std::vector<std::string> pos_string;
                boost::algorithm::split(pos_string, stations_position[i][0],
                                        boost::is_any_of(","));
                stations_data[j].position = {std::stof(pos_string[0]),
                                             std::stof(pos_string[1])};
                n++;
            }
        }
    }

    l << "Add position for " << n << "/" << order_ << " stations.\n";
}

int Graph::addLine(std::string type, std::string code, std::string color)
{
    lines_data.push_back({type, code, color});
    return lines_data.size() - 1;
}

int Graph::addStation(std::string name, float population, float employment,
                      Pos position)
{
    auto v = stations_data;
    auto it = std::find_if(v.begin(), v.end(), [&name](const StationData& s) {
        return s.name == name;
    });

    if (it != v.end())
    {
        // Log l(__FUNCTION__, true);
        // l << "La station " << name << " est deja presente dans
        // stations_data\n";
        return std::distance(v.begin(), it);
    } else
    {
        int index = stations_data.size();
        stations_data.push_back({name, population, employment, position});
        return index;
    }
}

void Graph::initStations(CSV lines_csv, std::string type)
{
    for (std::string code : lines_csv[getTypeId(type)])
    {
        code = removeQuote(code);

        std::stringstream line_path;
        line_path << "data/stations/" << type << "/" << code << ".csv";
        CSVParser csv_parser(line_path.str());
        CSV stations = csv_parser.getData();

        // Check if Json File is OK
        if (stations.size() > 0)
        {
            int line_id = addLine(type, code, getLineColor(type, code));

            std::string src_name = removeQuote(stations[0][0]);
            int src_id = addStation(src_name, 5, 5, {0, 0});

            for (size_t i = 0; i < stations.size() - 1; i++)
            {
                std::string dst_name = removeQuote(stations[i + 1][0]);
                int dst_id = addStation(dst_name, 5, 5, {0, 0});
                addEdgePair(src_id, dst_id, line_id, getTypeDuration(type), 0);
                src_id = dst_id;
            }
        } else
        {
            Log l2(__FUNCTION__, true);
            l2 << "Cannot find " << line_path.str() << "\n";
        }
    }
}

void Graph::correctFailure()
{
    Log l(__FUNCTION__);
    l << "Magenta --> Gare du Nord\n";
    addEdgePair(getStationId("Magenta"), getStationId("Gare du Nord"), 0, 5, 0);
    addEdgePair(getStationId("Anvers"), getStationId("Funiculaire Gare basse"),
                0, 5, 0);
    addEdgePair(getStationId("Villejuif - Louis Aragon"),
                getStationId("Villejuif-Louis Aragon"), 0, 1.5, 0);
    addEdgePair(getStationId("Gare de Bondy"), getStationId("Bondy"), 0, 2, 0);
}

Graph::Graph()
{
    Log l(__FUNCTION__);
    l << "Generating Graph from data/..\n";
    order_ = 0;
    CSVParser csv_parser("data/lines.csv");
    CSV lines_csv = csv_parser.getData();

    if (!lines_csv.size())
    {
        Log l2(__FUNCTION__, true);
        l2 << "Cannot find lines.csv\n";
        throw;
    }

    l << "Import metros stations\n";
    initStations(lines_csv, "metros");
    l << "Import rers stations\n";
    initStations(lines_csv, "rers");
    l << "Import tramways stations\n";
    initStations(lines_csv, "tramways");
    // l << "Import bus stations\n";
    // initStations(lines_csv, "bus");

    addStationsPosition();

    correctFailure();
}
