#pragma once

#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

using CSV = std::vector<std::vector<std::string>>;

class CSVParser
{
    std::string filename_;
    std::string delimeter_;

public:
    CSVParser(std::string filename, std::string delimeter = ",")
        : filename_(filename)
        , delimeter_(delimeter)
    {}
    CSV getData();
};

CSV CSVParser::getData()
{
    std::ifstream file(filename_);
    CSV dataList;

    std::string line = "";
    while (getline(file, line))
    {
        std::vector<std::string> line_vec;
        boost::algorithm::split(line_vec, line, boost::is_any_of(delimeter_));
        dataList.push_back(line_vec);
    }
    file.close();

    return dataList;
}
