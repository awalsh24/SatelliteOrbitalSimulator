cmake_minimum_required(VERSION 3.10)
project(SatelliteOrbitalSimulator CXX)

set(CMAKE_CXX_STANDARD 11)

# Find SFML
find_package(SFML 2.5 REQUIRED graphics window system)

# Find CURL
find_package(CURL REQUIRED)

# Collect all .cc files in the SGP4 folder
file(GLOB SGP4_SOURCES
    "${CMAKE_SOURCE_DIR}/libs/SGP4/libsgp4/*.cc"
)

# Create the executable and include your .cpp files
add_executable(SatelliteOrbitalSimulator
    main.cpp
    HttpFetcher.cpp
    TLEParser.cpp
    SGP4Propagator.cpp
    ${SGP4_SOURCES}
)

# Include directories
target_include_directories(SatelliteOrbitalSimulator PRIVATE
    ${SFML_INCLUDE_DIRS}                  # SFML
    ${CMAKE_SOURCE_DIR}/libs/SGP4/libsgp4 # for #include "SGP4.h"
)

# Link SFML, CURL, and any other libraries
target_link_libraries(SatelliteOrbitalSimulator
    sfml-graphics
    sfml-window
    sfml-system
    ${CURL_LIBRARIES}
)

# Optionally, link additional directories if needed
link_directories(/opt/homebrew/lib)