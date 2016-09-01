#pragma once

#include <SFML/Graphics.hpp>

class Player
{
public:
    Player();

    /** \brief Set the absolute position <x, y>. */
    void set_position(sf::Vector2f position);

    /** \brief Move the given distance <x, y>. */
    void move(sf::Vector2f distance);

    /** \brief Poll the current position <x, y> */
    sf::Vector2f get_position();

    /** \brief Poll the renderable graphic */
    const sf::CircleShape& render();

private:
    sf::Vector2f position_;
    sf::CircleShape graphic_;
};
