#include "player.hpp"

Player::Player() :
    position_(0.f, 0.f),
    graphic_(50.f)
{ }

void
Player::set_position(sf::Vector2f position)
{
    position_ = position;
}

void
Player::move(sf::Vector2f distance)
{
    position_ += distance;
}

sf::Vector2f
Player::get_position()
{
    return position_;
}

const sf::CircleShape&
Player::render()
{
    graphic_.setPosition(position_);
    graphic_.setFillColor(sf::Color::Black);
    return graphic_;
}
