#pragma once

#include <SFML/Graphics.hpp>

#include "AABB.hpp"

class Projectile
{
public:
    Projectile(sf::Vector2f position, sf::Vector2f velocity) :
        position_(position),
        velocity_(velocity),
        dimensions_(10.f, 10.f),
        alive_(true),
        graphic_(dimensions_.x/2.f)
    {
        graphic_.setFillColor(sf::Color::Red);
    }

    sf::Vector2f get_position() const;
    void set_position(sf::Vector2f position);


    sf::Vector2f get_velocity() const;
    void set_velocity(sf::Vector2f velocity);

    void kill();
    bool is_alive() { return alive_; }
    void update();

    AABB get_AABB();

    const sf::CircleShape render();

private:
    sf::Vector2f position_;
    sf::Vector2f velocity_;
    sf::Vector2f dimensions_;

    bool alive_;

    sf::CircleShape graphic_;
};

