#include "utils.hh"

#include <algorithm>
#include <cctype>
#include <iostream>
#include <string>

std::string removeQuote(std::string s)
{
    s.erase(remove(s.begin(), s.end(), '\"'), s.end());
    return s;
}

static std::string format(std::string s)
{
    std::string str;
    auto tolower_fun = [](unsigned char c) { return std::tolower(c); };
    str.assign(s);

    std::transform(str.begin(), str.end(), str.begin(), tolower_fun);

    std::replace(str.begin(), str.end(), '-', ' ');
    /*std::replace(str.begin(), str.end(), 'é', 'e');
    std::replace(str.begin(), str.end(), 'è', 'e');
    std::replace(str.begin(), str.end(), 'â', 'a');
    std::replace(str.begin(), str.end(), 'à', 'a');
    std::replace(str.begin(), str.end(), 'ô', 'o');
    std::replace(str.begin(), str.end(), 'î', 'i');
    */

    s.erase(remove(s.begin(), s.end(), ' '), s.end());

    // std::cout << str;
    return str;
}

int compareAdvanced(std::string s1, std::string s2)
{
    return format(s1).compare(format(s2));
}
