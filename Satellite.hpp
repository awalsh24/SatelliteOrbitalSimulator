#ifndef SATELLITE_HPP
#define SATELLITE_HPP

#include <SFML/Graphics.hpp>
#include <cmath>

struct Satellite {
    sf::Vector2f position;  // in meters
    sf::Vector2f velocity;  // in meters/second
    float mass;
    sf::Vector2f acceleration;

    Satellite(sf::Vector2f pos, sf::Vector2f vel, float m)
        : position(pos), velocity(vel), mass(m), acceleration(0.f, 0.f) {}

    void update(float deltaTime) {
        velocity += acceleration * deltaTime;
        position += velocity * deltaTime;
        acceleration = sf::Vector2f(0.f, 0.f); // Reset acceleration after each update
    }

    void calculate_gravity(const sf::Vector2f& earth_position, float earth_mass) {
        // Gravitational constant
        const float G = 6.67430e-11f;
        sf::Vector2f diff = earth_position - position;
        float r = std::sqrt(diff.x * diff.x + diff.y * diff.y);
        
        float force_mag = (G * earth_mass * mass) / (r * r);
        sf::Vector2f force_dir = diff / r;
        
        // a = F/m
        sf::Vector2f accel = force_dir * (force_mag / mass);
        acceleration += accel;
    }
};

#endif