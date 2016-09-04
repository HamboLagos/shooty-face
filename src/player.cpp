#include "player.hpp"

#include <cmath>

Player::Player() :
    graphic_(),
    projectile_speed_(0.f),
    projectile_(nullptr)
{ }

Player::~Player()
{
    if (projectile_ != nullptr) {
        delete projectile_;
    }
}

void
Player::update(sf::Time elapsed)
{
    float dt = elapsed.asSeconds();
    float dx = 0.f;
    float dy = 0.f;

    if (is_moving.left && !is_moving.right) {
        dx = -get_velocity().x * dt;
    } else if (is_moving.right && !is_moving.left) {
        dx = get_velocity().x * dt;
    }

    if (is_moving.up && !is_moving.down) {
        dy = -get_velocity().y * dt;
    } else if (is_moving.down && !is_moving.up) {
        dy = get_velocity().y * dt;
    }

    move({dx, dy});

    if (projectile_ != nullptr) {
        if (projectile_->is_alive()) {
            projectile_->update(elapsed);
        } else {
            delete projectile_;
            projectile_ = nullptr;
        }
    }
}

const sf::Drawable&
Player::render()
{
    graphic_.setSize(get_dimensions());
    graphic_.setOrigin(get_extents());
    graphic_.setPosition(get_position());
    graphic_.setFillColor(sf::Color::Black);
    return graphic_;
}

void
Player::start_move(Direction direction)
{
    is_moving.set(true, direction);
}

void
Player::stop_move(Direction direction)
{
    is_moving.set(false, direction);
}

void Player::shoot(sf::Vector2f target)
{
    if (projectile_ != nullptr) {
        delete projectile_;
    }

    sf::Vector2f vector = target - get_position();
    float vector_length = sqrt(vector.x*vector.x + vector.y*vector.y);
    sf::Vector2f unit_vector = vector / vector_length;
    sf::Vector2f projectile_velocity = projectile_speed_ * unit_vector;

    projectile_ = new Projectile();
    projectile_->set_position(get_position());
    projectile_->set_velocity(projectile_velocity);
}

void Player::set_projectile_speed(float speed)
{
    projectile_speed_ = speed;
}

Projectile* Player::get_projectile() const
{
    return projectile_;
}

void
Player::MovementDirections::set(bool is_moving, Direction direction)
{
    switch (direction) {

    case Direction::UP:
        up = is_moving;
        break;

    case Direction::LEFT:
        left = is_moving;
        break;

    case Direction::DOWN:
        down = is_moving;
        break;

    case Direction::RIGHT:
        right = is_moving;
        break;

    default:
        break;
    }
}
