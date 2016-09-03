#pragma once

#include <SFML/Graphics.hpp>

#include "AABB.hpp"

class Enemy
{
public:
    Enemy();

    void update();

    AABB get_AABB();

    void set_position(sf::Vector2f position);
    void set_move_velocity(sf::Vector2f velocity);
    sf::Vector2f get_move_velocity() { return velocity_; }

    void move(sf::Vector2f distance);

    void start();
    void stop();

    bool is_running();

    sf::RectangleShape render();

private:
    bool running_;

    sf::Vector2f position_;
    sf::Vector2f dimensions_;
    sf::Vector2f velocity_;
    sf::RectangleShape graphic_;
};
