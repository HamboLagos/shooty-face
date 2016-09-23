#pragma once

#include <utility>

#include <SFML/System/Vector2.hpp>

namespace util {

struct VectorParts
{
    VectorParts(sf::Vector2f dir = {0.f, 0.f}, float mag = 0.f) :
        direction(dir),
        length(mag)
    { }

    sf::Vector2f direction;
    float length;
};

/** \brief Truncates the float values in the given Vector2f. */
sf::Vector2f pixelate(sf::Vector2f vector);

/** \brief Gets the unit vector from the given vector. */
sf::Vector2f direction(sf::Vector2f vector);

/** \brief Gets the magnitude of the given vector. */
float length(sf::Vector2f vector);

/** \brief Gets the direction and length of the given vector. */
VectorParts devector(sf::Vector2f vector);

/** \brief Constructs a vector from the given direction and length. */
sf::Vector2f revector(VectorParts dir_len);

}
