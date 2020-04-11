#include "utils.hh"

#include <algorithm>
#include <string>

std::string removeQuote(std::string s)
{
    s.erase(remove(s.begin(), s.end(), '\"'), s.end());
    return s;
}
