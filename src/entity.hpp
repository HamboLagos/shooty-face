#pragma once

#include <SFML/System/Time.hpp>

#include "AABB.hpp"
#include "graphical.hpp"

/** \brief Entity is a base for things which have dimension. */
class Entity {
public:
    Entity();
    virtual ~Entity() = default;

    inline void set_dimensions(sf::Vector2f dimensions) { dimensions_ = dimensions; }
    inline void enlarge(sf::Vector2f bigness) { dimensions_ += bigness; }
    inline void scale(float scale) { dimensions_ *= scale; }
    inline void scale(sf::Vector2f scale) { dimensions_.x *= scale.x; dimensions_.y *= scale.y; }
    inline sf::Vector2f get_dimensions(void) const { return dimensions_; }
    inline sf::Vector2f get_extents(void) const { return dimensions_/2.f; }

    inline void set_position(sf::Vector2f position) { position_ = position; }
    inline void move(sf::Vector2f distance) { position_ += distance; }
    inline sf::Vector2f get_position() const { return position_; }

    inline void set_velocity(sf::Vector2f velocity) { velocity_ = velocity; }
    inline void speed_up(sf::Vector2f acceleration) { velocity_ += acceleration; }
    inline sf::Vector2f get_velocity() const { return velocity_; }

    inline void set_alive(bool is_alive) { is_alive_ = is_alive; }
    inline void kill() { is_alive_ = false; }
    inline bool is_alive() const { return is_alive_; }
    inline bool is_dead() const { return !is_alive_; }

    inline void set_solid(bool solid) { is_solid_ = solid; }
    inline bool is_solid() const { return is_solid_; }
    inline bool is_passable() const { return !is_solid_; }

    /** \brief Get the AABB for this Entity.
     *
     * The AABB is invalidated by a change to this Entity's dimensions, position, or velocity. To
     * ensure memory-safety, the AABB is never cached, but is recalculated on each call. */
    inline AABB get_box(sf::Time elapsed = sf::Time::Zero) const
    { return AABB(position_, dimensions_, velocity_ * elapsed.asSeconds()); }

    /** \brief Update this entity.
     *
     * Typically called once per main-loop tick.
     *
     * \param[in] dt Time delta to perform update over, decouples game logic from framerate. */
    virtual void update(sf::Time elapsed) = 0;

    virtual inline bool operator==(const Entity& other) const
    {
        return
            dimensions_ == other.dimensions_ &&
            position_   == other.position_ &&
            velocity_   == other.velocity_ &&
            is_alive_   == other.is_alive_ &&
            is_solid_   == other.is_solid_;
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
    bool is_solid_; ///< Solid entities are "impassable"
};

/** \brief GraphicalEntity is a base for things which are drawn on screen. */
class GraphicalEntity : public Entity, public Graphical
{
public:
    GraphicalEntity() = default;
    virtual ~GraphicalEntity() = default;

    virtual void update(sf::Time elapsed) = 0;
    virtual void render() = 0;
};
