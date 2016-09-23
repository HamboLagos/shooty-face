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
    auto magnitude = length(vector);

    if (magnitude != 0.f) {
        return vector / magnitude;
    } else {
        return vector;
    }
}

float
length(sf::Vector2f vector)
{
    return sqrt(vector.x*vector.x + vector.y*vector.y);
}

VectorParts devector(sf::Vector2f vector)
{
    return VectorParts(direction(vector), length(vector));
}

sf::Vector2f revector(VectorParts parts)
{
    return parts.direction * parts.length;
}

}
