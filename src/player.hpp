#pragma once

#include <SFML/Graphics/RectangleShape.hpp>

#include "entity.hpp"
#include "gun.hpp"

class Player : public Entity
{
friend class TestablePlayer;
public:
    enum class Direction
    {
        RIGHT,
        DOWN,
        LEFT,
        UP
    };

    Player();

    void update(sf::Time dt) override;
    const sf::Drawable& render() override;

    /** \brief Apply velocity to the player in the given direction.
     *
     * Stop the movement with stop_move(). Movements in the opposite direction cancel each other
     * out, while active. Movements speeds do not stack when applied more than once. */
    void start_move(Direction direction);

    /** \brief Stop movement in the given direction.
     *
     * Cancels the movement in the given direction. If the player was not already moving in the
     * given direction, has no effect. */
    void stop_move(Direction direction);

    Gun& get_gun() { return gun_; }

private:
    sf::RectangleShape graphic_;

    Gun gun_;

    struct MovementDirections
    {
        bool up    = false;
        bool left  = false;
        bool down  = false;
        bool right = false;

        void set(bool is_moving, Direction direction);
    } is_moving;
};
