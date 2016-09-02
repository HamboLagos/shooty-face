#pragma once

#include <SFML/Graphics.hpp>

class Player
{
public:
    enum class Direction
    {
        UP,
        LEFT,
        DOWN,
        RIGHT
    };

    class Projectile
    {
    public:
        Projectile(sf::Vector2f position, sf::Vector2f velocity) :
            position_(position),
            velocity_(velocity),
            graphic_(10.f)
        {
            graphic_.setFillColor(sf::Color::Red);
        }

        sf::Vector2f get_position() const { return position_; }
        void set_position(sf::Vector2f position)
        { position_ = position; }

        sf::Vector2f get_velocity() const { return velocity_; }
        void set_velocity(sf::Vector2f velocity)
        {  velocity_ = velocity; }

        void update() { position_ += velocity_; }

        const sf::CircleShape render()
        {
            graphic_.setPosition(position_);
            return graphic_;
        }

    private:
        sf::Vector2f position_;
        sf::Vector2f velocity_;

        sf::CircleShape graphic_;
    };

    Player();

    void update();

    /** \brief Set the absolute position <x, y>. */
    void set_position(sf::Vector2f position);

    /** \brief Set the <x, y> velocity of the player.
     *
     * X and Y velocities are independent of each other.*/
    void set_velocity(sf::Vector2f velocity);

    /** \brief Move the given distance <x, y>. */
    void move(sf::Vector2f distance);

    /** \brief Apply velocity change in the given direction.
     *
     * Stop the movement with stop_move(). Movements in the opposite direction cancel each other
     * out, while active.*/
    void start_move(Direction direction);

    /** \brief Stop movement in the given direction. */
    void stop_move(Direction direction);

    /** \brief Poll the current position <x, y>. */
    sf::Vector2f get_position() const;

    void shoot(sf::Vector2i target);

    void set_projectile_speed(float speed);

    Projectile* get_projectile() const;

    /** \brief Get the renderable graphic for the player. */
    const sf::RectangleShape& render();

private:
    sf::Vector2f position_;
    sf::Vector2f velocity_;
    sf::RectangleShape graphic_;

    float projectile_speed_;
    Projectile* projectile_;

    bool moving_up_;
    bool moving_left_;
    bool moving_down_;
    bool moving_right_;
};
