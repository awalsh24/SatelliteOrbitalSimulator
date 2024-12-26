#ifndef TLEPARSER_HPP
#define TLEPARSER_HPP

#include <string>
#include <vector>

struct TLE {
    std::string name;
    std::string line1;
    std::string line2;
};

std::vector<TLE> parseTLEData(const std::string& data);

#endif