#pragma once

#include <utility>

#include <SFML/System/Vector2.hpp>

namespace util {

using VectorParts = std::pair<sf::Vector2f, float>; ///< direction, length

/** \brief Truncates the float values in the given Vector2f. */
sf::Vector2f pixelate(sf::Vector2f vector);

/** \brief Gets the unit vector from the given vector. */
sf::Vector2f direction(sf::Vector2f vector);

/** \brief Gets the magnitude of the given vector. */
float length(sf::Vector2f vector);

/** \brief Gets the unit_vector and magnitude of the given vector. */
VectorParts devector(sf::Vector2f vector);

/** \brief Constructs a vector from the given direction and magnitude. */
sf::Vector2f revector(VectorParts dir_len);

}
