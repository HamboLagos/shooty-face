#include "utils.hpp"

sf::Vector2f pixelate(sf::Vector2f vector)
{
    return sf::Vector2f(sf::Vector2i(vector));
}
