#ifndef SGP4PROPAGATOR_HPP
#define SGP4PROPAGATOR_HPP

#include <string>
#include "TLEParser.hpp" // for TLE struct

// Holds the (x, y, z) position in meters
struct SatellitePosition {
    double x;
    double y;
    double z;
};

class SGP4Propagator {
public:
    explicit SGP4Propagator(const TLE& tle);
    SatellitePosition propagate(double minutesSinceEpoch);

private:
    std::string line1_;
    std::string line2_;
    std::string name_;
};

#endif // SGP4PROPAGATOR_HPP