#include "TLEParser.hpp"
#include <sstream>
#include <algorithm>
#include <cctype>

std::vector<TLE> parseTLEData(const std::string& data) {
    std::vector<TLE> satellites;
    std::istringstream iss(data);
    std::string line;
    TLE current;

    while (std::getline(iss, line)) {
        // Trim whitespace from both ends
        line.erase(line.begin(), std::find_if(line.begin(), line.end(), [](unsigned char ch) {
            return !std::isspace(ch);
        }));
        line.erase(std::find_if(line.rbegin(), line.rend(), [](unsigned char ch) {
            return !std::isspace(ch);
        }).base(), line.end());

        if (line.empty()) continue;

        // Check the first character to determine line type
        if (line.size() > 0 && line[0] == '1' && line.size() > 50) {
            // This is TLE line 1
            current.line1 = line;
        } else if (line.size() > 0 && line[0] == '2' && line.size() > 50) {
            // This is TLE line 2
            current.line2 = line;
            satellites.push_back(current);
            current = TLE(); // Reset for next satellite
        } else {
            // Assume this is a name line
            current.name = line;
        }
    }

    return satellites;
}