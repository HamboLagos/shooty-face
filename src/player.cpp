#include "player.hpp"

#include <cmath>

Player::Player() :
    position_(0.f, 0.f),
    dimensions_(50.f, 50.f),
    velocity_(0.f, 0.f),
    graphic_(dimensions_),
    projectile_(nullptr),
    moving_up_(false),
    moving_left_(false),
    moving_down_(false),
    moving_right_(false)
{
    graphic_.setOrigin(get_AABB().get_extents()); // set the origin to the center
    graphic_.setFillColor(sf::Color::Black);
}

void
Player::update()
{
    float dy = 0.f;
    float dx = 0.f;

    if (moving_up_ && !moving_down_) {
        dy = -velocity_.y;
    } else if (moving_down_ && !moving_up_) {
        dy = velocity_.y;
    }

    if (moving_left_ && !moving_right_) {
        dx = -velocity_.x;
    } else if (moving_right_ && !moving_left_) {
        dx = velocity_.x;
    }

    move({dx, dy});

    if (projectile_ != nullptr) {
        projectile_->update();
    }
}

AABB
Player::get_AABB()
{
    return AABB(position_, dimensions_);
}

void
Player::set_position(sf::Vector2f position)
{
    position_ = position;
}

void
Player::set_move_velocity(sf::Vector2f velocity)
{
    velocity_ = velocity;
}

void
Player::move(sf::Vector2f distance)
{
    position_ += distance;
}

void
Player::start_move(Player::Direction direction)
{
    switch (direction) {

    case Direction::UP:
        moving_up_ = true;
        break;

    case Direction::LEFT:
        moving_left_ = true;
        break;

    case Direction::DOWN:
        moving_down_ = true;
        break;

    case Direction::RIGHT:
        moving_right_ = true;
        break;

    default:
        break;
    }
}

void
Player::stop_move(Player::Direction direction)
{
    switch (direction) {

    case Direction::UP:
        moving_up_ = false;
        break;

    case Direction::LEFT:
        moving_left_ = false;
        break;

    case Direction::DOWN:
        moving_down_ = false;
        break;

    case Direction::RIGHT:
        moving_right_ = false;
        break;

    default:
        break;
    }
}

sf::Vector2f
Player::get_position() const
{
    return position_;
}

void Player::shoot(sf::Vector2i target)
{
    if (projectile_ != nullptr) {
        delete projectile_;
    }

    sf::Vector2f vector = sf::Vector2f(target) - get_position();
    float vector_length = sqrt(vector.x*vector.x + vector.y*vector.y);
    sf::Vector2f unit_vector = vector / vector_length;
    sf::Vector2f projectile_velocity = projectile_speed_ * unit_vector;

    projectile_ = new Projectile(get_position(), projectile_velocity);
}

void Player::set_projectile_speed(float speed)
{
    projectile_speed_ = speed;
}

Projectile* Player::get_projectile() const
{
    return projectile_;
}

const sf::RectangleShape&
Player::render()
{
    graphic_.setPosition(position_);
    return graphic_;
}
