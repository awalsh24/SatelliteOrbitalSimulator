#include <SFML/Graphics.hpp>
#include <cmath>
#include <iostream>
#include <array>
#include <sstream>

// libsgp4 headers for lat/lon conversion
#include "DateTime.h"
#include "CoordGeodetic.h"
#include "Eci.h"
#include "Vector.h"
#include "Tle.h"
#include "SGP4.h"  // For Tle::Epoch(), Eci.ToGeodetic(), etc.
using namespace libsgp4;

// Custom headers
#include "HttpFetcher.hpp"
#include "TLEParser.hpp"
#include "SGP4Propagator.hpp"

// Constants
static const double EarthMass = 5.972e24;
static const sf::Vector2f EarthPosition(0.f, 0.f);

int main() {
    // 1. Fetch TLE data
    std::string url = "https://celestrak.org/NORAD/elements/stations.txt";
    std::string tleData = fetchTLE(url);
    if (tleData.empty()) {
        std::cerr << "Failed to fetch TLE data.\n";
        return 1;
    }

    // 2. Parse TLE data
    auto satellites = parseTLEData(tleData);
    if (satellites.empty()) {
        std::cerr << "No satellites found in TLE data.\n";
        return 1;
    }

    // Pick up to 5 satellites
    const int count = std::min((int)satellites.size(), 5);

    // Create propagators
    std::vector<SGP4Propagator> propagators;
    propagators.reserve(count);

    // We'll also store the Tle objects themselves to get each satellite's epoch
    std::vector<Tle> tleObjects;
    tleObjects.reserve(count);

    for (int i = 0; i < count; ++i) {
        propagators.emplace_back(satellites[i]);
        // Build a libsgp4::Tle object for each
        tleObjects.emplace_back(satellites[i].name, satellites[i].line1, satellites[i].line2);
        std::cout << "Using satellite: " << satellites[i].name << "\n";
    }

    // 3. Create an SFML window
    sf::RenderWindow window(sf::VideoMode(800, 600), "Orbital Simulator");
    window.setFramerateLimit(60);

    // Load a font for text
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Failed to load font 'arial.ttf'. Make sure it's in the project folder.\n";
        // handle error or return 1;
    }

    // Scale factor: 1 pixel = 50,000 meters
    const double SCALE = 50000.0;
    sf::Vector2f screenCenter(400.f, 300.f);

    // Earth shape
    sf::CircleShape earthShape(30.f);
    earthShape.setFillColor(sf::Color::Blue);
    earthShape.setOrigin(30.f, 30.f);
    earthShape.setPosition(screenCenter);

    // Create 5 colors for the satellites
    std::array<sf::Color, 5> colors = {
        sf::Color::Red,
        sf::Color::Green,
        sf::Color::Yellow,
        sf::Color::Cyan,
        sf::Color(255, 105, 180) // Hot Pink
    };

    // Create satellite shapes (same size, different colors)
    std::vector<sf::CircleShape> satelliteShapes(count);
    for (int i = 0; i < count; ++i) {
        satelliteShapes[i].setRadius(5.f);
        satelliteShapes[i].setFillColor(colors[i]);
        satelliteShapes[i].setOrigin(5.f, 5.f);
    }

    // Create text objects for each satellite
    std::vector<sf::Text> satelliteTexts(count);
    for (int i = 0; i < count; ++i) {
        satelliteTexts[i].setFont(font);
        satelliteTexts[i].setCharacterSize(14);
        satelliteTexts[i].setFillColor(sf::Color::White);
        // We'll set the string every frame in the loop
    }

    // Create a sidebar background rectangle (200px wide)
    sf::RectangleShape sidebar(sf::Vector2f(200.f, 600.f));
    sidebar.setPosition(600.f, 0.f);
    sidebar.setFillColor(sf::Color(50, 50, 50)); // Dark gray

    // Time since TLE epoch
    double totalMinutes = 0.0;
    sf::Clock clock;

    // 4. Main Loop
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Update the time
        float deltaTime = clock.restart().asSeconds();
        totalMinutes += (deltaTime / 60.0);

        window.clear(sf::Color::Black);

        // Draw Earth
        window.draw(earthShape);

        // For each satellite
        for (int i = 0; i < count; ++i) {
            // Propagate ECI position
            SatellitePosition pos = propagators[i].propagate(totalMinutes);

            // Convert x, y from meters to pixels
            float drawX = static_cast<float>(screenCenter.x + (pos.x / SCALE));
            float drawY = static_cast<float>(screenCenter.y + (pos.y / SCALE));

            satelliteShapes[i].setPosition(drawX, drawY);
            window.draw(satelliteShapes[i]);

            // -------- Compute latitude/longitude using libsgp4 --------
            // Build an SGP4 + Eci again to get lat/lon, or just do direct:
            // We already have Tle & time. So let's do that:

            Tle& thisTleObj = tleObjects[i]; 
            SGP4 sgp4(thisTleObj);  // Reconstruct each time, or store it
            Eci eci = sgp4.FindPosition(totalMinutes);
            
            // We must compute the actual DateTime
            // Start with the TLE epoch
            DateTime dt = thisTleObj.Epoch();
            // Add the time we've advanced in our simulation
            dt.AddSeconds(totalMinutes * 60.0);

            CoordGeodetic geo = eci.ToGeodetic();

            // Convert to degrees for readability
            double latDeg = Util::RadiansToDegrees(geo.latitude);
            double lonDeg = Util::RadiansToDegrees(geo.longitude);

            // Build the text string
            std::ostringstream info;
            info << satellites[i].name << "\n"
                 << "Lat: " << latDeg << "\n"
                 << "Lon: " << lonDeg;

            // Update the text object
            satelliteTexts[i].setString(info.str());
            // Position each text chunk with some spacing
            satelliteTexts[i].setPosition(610.f, 20.f + i * 100.f);
        }

        // Draw the sidebar
        window.draw(sidebar);

        // Draw the text for each satellite on top of the sidebar
        for (int i = 0; i < count; ++i) {
            window.draw(satelliteTexts[i]);
        }

        window.display();
    }

    return 0;
}