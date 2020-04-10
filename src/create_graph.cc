#include <climits>
#include <fstream>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <utility>

#include "graph.hh"
#include "json.hpp"

using json = nlohmann::json;

static json stream_to_json(const std::string& filename)
{
    std::cout << "parsing " << filename << "...\n";
    std::ifstream s(filename);

    json j;
    try
    {
        s >> j;
    } catch (std::exception& e)
    {
        std::cerr << "Error Parsing Json: cannot load " << filename << "\n";
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
            auto station = get_station(g->station_list, l[i]["slug"]);
            std::stringstream line_id;
            line_id << type << code;
            if (!station)
                std::cerr << "error no station named " << l[i]["slug"] << "\n";
            else
                fill_adj_station(g->station_list, station, l, line_id.str(), i);
        }
    }
}*/

/*

static void update_id(std::shared_ptr<Graph> g)
{
    for (size_t i = 0; i < v.size(); i++)
        v[i]->set_id(i);
}*/

static void init_stations(std::shared_ptr<Graph> g, json lines_json,
                          const std::string& type)
{
    std::cout << "Init stations list...\n";

    for (auto l : lines_json["result"][type])
    {
        std::stringstream line_path;
        std::string code(l["code"]);
        code.erase(remove(code.begin(), code.end(), '\"'), code.end());
        line_path << "data/stations/" << type << "/" << code << ".json";
        json line = stream_to_json(line_path.str());
        for (auto s_js : line["result"]["stations"])
        {
            std::cout << "Adding station " << s_js["slug"] << "\n";

            /*auto station = get_station(g->station_list, s_js["slug"]);
            if (!station)
            {
                std::shared_ptr<Station> station =
                    std::make_shared<Station>(s_js["name"], s_js["slug"]);
                std::stringstream line_id;
                line_id << type << code;

                station->add_line(line_id.str());
                g->station_list.push_back(station);
            } else
            {
                std::stringstream line_id;
                line_id << type << code;
                station->add_line(line_id.str());
            }*/
        }
    }
}

static void correct_failure(std::shared_ptr<Graph> g)
{
    (void)g;
    std::cout << "[2/3] Correct Graph Errors\n";
}

Graph::Graph(std::string& folder_path)
{
    json lines_json = stream_to_json(folder_path << "/lines.json");
    init_stations(g, lines_json, "metros");
    init_stations(g, lines_json, "rers");
    init_stations(g, lines_json, "tramways");

    // update_adj_lists(g, "metros");
    // update_adj_lists(g, "rers");
    // update_adj_lists(g, "tramways");
    // update_adj_lists(g, "bus");

    // update_id(g->station_list);

    order_ = 3000;
    adj_list.resize(order);

    correct_failure(g->station_list);

    /*
    Log log("Adjency");
    for (auto s : g->station_list)
    {
        for (auto l : s->get_lines())
            log << s->get_name() << " avec la " << l << "\n";
        for (auto a : s->get_adjs())
            log << "		ajdacent a " << a.second.first->get_name()
                << " sur la ligne " << a.first << "\n";
    }*/

    return g;
}
