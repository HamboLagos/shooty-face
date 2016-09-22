#pragma once

#include <SFML/Graphics/RectangleShape.hpp>

#include "components/graphics.hpp"
#include "entity.hpp"

class Player : public Entity, public Renderer
{
public:
    static constexpr float SPEED = 250.f;

    enum class Direction
    {
        RIGHT,
        DOWN,
        LEFT,
        UP
    };

    Player();
    virtual ~Player() = default;

    sf::Time update(sf::Time elapsed) override;
    const Renderings render() override;

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

private:
    sf::RectangleShape graphic_;

    struct MovementDirections
    {
        bool up    = false;
        bool left  = false;
        bool down  = false;
        bool right = false;

        void set(bool is_moving, Direction direction);
    } is_moving;
};
