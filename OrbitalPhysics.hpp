#ifndef ORBITALPHYSICS_HPP
#define ORBITALPHYSICS_HPP

// OrbitalPhysics.hpp

const double G = 6.67430e-11;  // Gravitational constant (m^3 kg^-1 s^-2)
const double EarthMass = 5.972e24; // Mass of the Earth (kg)
const double EarthRadius = 6371000; // Radius of the Earth (m)

#include "Satellite.hpp"

// Update the satellite's position based on its velocity and elapsed time
void updateSatellite(Satellite& satellite, float deltaTime) {
    satellite.position += satellite.velocity * deltaTime;
}

#endif