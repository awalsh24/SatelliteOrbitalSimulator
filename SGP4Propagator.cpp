#include "SGP4Propagator.hpp"
#include "SGP4.h"      // from libs/SGP4/libsgp4
#include "Tle.h"
#include "Eci.h"
#include "Vector.h"

using namespace libsgp4; // So we can refer to SGP4, Tle, Eci, etc.

SGP4Propagator::SGP4Propagator(const TLE& tle)
    : line1_(tle.line1)
    , line2_(tle.line2)
    , name_(tle.name)
{
}

SatellitePosition SGP4Propagator::propagate(double minutesSinceEpoch) {
    // Create a Tle object
    Tle tleObj(name_, line1_, line2_);

    // Construct an SGP4 object from the Tle
    SGP4 sgp4Object(tleObj);

    // Get the ECI position (in km)
    Eci eci = sgp4Object.FindPosition(minutesSinceEpoch);
    Vector pos = eci.Position(); // pos.x, pos.y, pos.z in km

    // Convert km to meters
    SatellitePosition satPos;
    satPos.x = pos.x * 1000.0;
    satPos.y = pos.y * 1000.0;
    satPos.z = pos.z * 1000.0;

    return satPos;
}