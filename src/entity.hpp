#pragma once

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Drawable.hpp>

#include "AABB.hpp"

class Entity {
public:
    /** \brief Creates a new Entity.
     *
     * \param[in] dimensions The dimensions of this entity, defines its AABB.
     * \param[in] position The initial position of this entity, geometrically centered.
     * \param[in] velocity The initial velocity of this entity. */
    Entity(sf::Vector2f dimensions = {0.f, 0.f},
           sf::Vector2f position = {0.f, 0.f},
           sf::Vector2f velocity = {0.f, 0.f}) :
        dimensions_(dimensions),
        position_(position),
        velocity_(velocity),
        is_alive_(true)
    { }

    virtual ~Entity() = default;

    inline sf::Vector2f get_dimensions(void) const { return dimensions_; }
    inline sf::Vector2f get_extents(void) const { return dimensions_/2.f; }
    inline void set_dimensions(sf::Vector2f dimensions) { dimensions_ = dimensions; }
    inline void enlarge(sf::Vector2f bigness) { dimensions_ += bigness; }
    inline void scale(float scale) { dimensions_ *= scale; }
    inline void scale(sf::Vector2f scale) { dimensions_.x *= scale.x; dimensions_.y *= scale.y; }

    inline sf::Vector2f get_position() const { return position_; }
    inline void set_position(sf::Vector2f position) { position_ = position; }
    inline void move(sf::Vector2f distance) { position_ += distance; }

    inline sf::Vector2f get_velocity() const { return velocity_; }
    inline void set_velocity(sf::Vector2f velocity) { velocity_ = velocity; }
    inline void speed_up(sf::Vector2f acceleration) { velocity_ += acceleration; }

    inline void animate() { is_alive_ = true; }
    inline void kill() { is_alive_ = false; }
    inline bool is_alive() const { return is_alive_; }
    inline bool is_dead() const { return !is_alive_; }

    /** \brief Get the AABB for this Entity.
     *
     * The AABB is invalidated by a change to this Entity's dimensions or position. To ensure
     * memory-safety, the AABB is recalculated on each call. */
    inline AABB get_box() const { return AABB(position_, dimensions_); }

    /** \brief Update this entity.
     *
     * Typically called once per main-loop tick.
     *
     * \param[in] dt Time delta to perform update over, decouples game logic from framerate. */
    virtual void update(sf::Time elapsed) = 0;

    /** \brief Get the drawable graphic for this Entity.
     *
     * The Entity class does not allocate storage space for graphics, since it varies by client. */
    virtual const sf::Drawable& render() = 0;

    virtual inline bool operator==(const Entity& other) const
    {
        return
            dimensions_ == other.dimensions_ &&
            position_   == other.position_ &&
            velocity_   == other.velocity_ &&
            is_alive_   == other.is_alive_;
    }

    virtual inline bool operator!=(const Entity& other) const
    {
        return !(*this == other);
    }

private:
    sf::Vector2f dimensions_; ///< <width, height> of this entity, defines AABB
    sf::Vector2f position_; ///< Current <x, y> position
    sf::Vector2f velocity_; ///< Current <vx, vy> velocity
    bool is_alive_; ///< "Aliveness" changes depending on the client context
};

