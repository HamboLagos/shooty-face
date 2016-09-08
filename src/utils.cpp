#include <cmath>

#include "utils.hpp"

namespace util {

sf::Vector2f
pixelate(sf::Vector2f vector)
{
    return sf::Vector2f(sf::Vector2i(vector));
}

sf::Vector2f
direction(sf::Vector2f vector)
{
    return vector / length(vector);
}

float
length(sf::Vector2f vector)
{
    return sqrt(vector.x*vector.x + vector.y*vector.y);
}

VectorParts devector(sf::Vector2f vector)
{
    return std::make_pair(direction(vector), length(vector));
}

sf::Vector2f revector(VectorParts parts)
{
    return parts.first * parts.second;
}

}
