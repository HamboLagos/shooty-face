#include "enemy.hpp"

Enemy::Enemy() :
    running_(false),
    position_({0.f, 0.f}),
    dimensions_(20.f, 20.f),
    velocity_(0.f, 0.f),
    graphic_(dimensions_)
{
    graphic_.setOrigin(get_AABB().get_extents());
    graphic_.setFillColor(sf::Color::Green);
}

void
Enemy::update()
{
    if (running_) {
        move(velocity_);
    }
}

AABB
Enemy::get_AABB()
{
    return AABB(position_, dimensions_);
}

void
Enemy::set_position(sf::Vector2f position)
{
    position_ = position;
}

void
Enemy::set_move_velocity(sf::Vector2f velocity)
{
    velocity_ = velocity;
}

void
Enemy::move(sf::Vector2f distance)
{
    position_ += velocity_;
}

void
Enemy::start()
{
    running_ = true;
}

void
Enemy::stop()
{
    running_ = false;
}

bool
Enemy:: is_running()
{
    return running_;
}

sf::RectangleShape
Enemy::render()
{
    graphic_.setPosition(position_);
    return graphic_;
}
