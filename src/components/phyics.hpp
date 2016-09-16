#pragma once

#include "component.hpp"
#include "../AABB.hpp"

class Physics : public Component
{
public:
    Physics(Entity& entity) :
        Component(entity),
        move_speed_(0.f),
        is_solid_(true),
        is_static_(false)
    { }

    virtual ~Physics() = default;

    inline void set_position(sf::Vector2f position) { position_ = position; }
    inline void move(sf::Vector2f distance) { position_ += distance; }
    inline sf::Vector2f get_position() const { return position_; }

    inline void set_velocity(sf::Vector2f velocity) { velocity_ = velocity; }
    inline sf::Vector2f get_velocity() const { return velocity_; }

    inline sf::Vector2f get_trajectory(sf::Time elapsed) const
    { return velocity_ * elapsed.asSeconds(); }

    inline void set_move_speed(float move_speed) { move_speed_ = move_speed; }
    inline float get_move_speed() const { return move_speed_; }

    inline void set_dimensions(sf::Vector2f dimensions) { dimensions_ = dimensions; }
    inline void enlarge(sf::Vector2f bigness) { dimensions_ += bigness; }
    inline void scale(float scale) { dimensions_ *= scale; }
    inline void scale(sf::Vector2f scale) { dimensions_.x *= scale.x; dimensions_.y *= scale.y; }
    inline sf::Vector2f get_dimensions(void) const { return dimensions_; }
    inline sf::Vector2f get_extents(void) const { return dimensions_/2.f; }

    inline void set_solid(bool solid) { is_solid_ = solid; }
    inline bool is_solid() const { return is_solid_; }
    inline bool is_passable() const { return !is_solid_; }

    inline void set_static(bool is_static) { is_static_ = is_static; }
    inline bool is_static() const { return is_static_; }
    inline bool is_dynamic() const { return !is_static_; }

    /** \brief Get the AABB for this Entity.
     *
     * The AABB is invalidated by a change to this Entity's dimensions, position, or trajectory. To
     * ensure memory-safety, the AABB is never cached, but is recalculated on each call. */
    inline AABB get_box(sf::Time elapsed = sf::Time::Zero) const
    { return AABB(position_, dimensions_, get_trajectory(elapsed)); }

private:
    sf::Vector2f position_; ///< Current <x, y> position
    sf::Vector2f velocity_; ///< Current <vx, vy> velocity
    float move_speed_;      ///< Magnitude of velocity

    sf::Vector2f dimensions_; ///< <width, height> of this entity

    bool is_solid_;  ///< Solid entities are "impassable"
    bool is_static_; ///< Static entities are immovable
};
