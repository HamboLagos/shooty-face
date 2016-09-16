#pragma once

#include <memory>
#include <unordered_map>
#include <typeindex>

#include <SFML/System/Time.hpp>

#include "AABB.hpp"

class Component
{
public:
    Component() = default;

    virtual ~Component() = default;
};

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

    /** \brief Checks if this entity has this component type. */
    template<class T> bool has_component();

    /** \brief Default constructs and returns a component of type T. */
    template<class T> T* add_component();

    /** \brief Copy constructs and returns the component of type T. */
    template<class T> T* set_component(std::unique_ptr<Component> component);

    /** \brief Retrieves the component for this entity of type T, or nullptr. */
    template<class T> T* get_component();

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

    std::unordered_map<std::type_index, std::unique_ptr<Component>> components_;
};

template<class T>
bool
Entity::has_component()
{
    static_assert(std::is_base_of<Component, T>(), "Require Component Subclass");

    return components_.count(std::type_index(typeid(T))) != 0;
}

template<class T>
T*
Entity::add_component()
{
    static_assert(std::is_base_of<Component, T>(), "Require Component Subclass");

    auto& ptr = components_[std::type_index(typeid(T))];
    ptr.reset(new T());

    return static_cast<T*>(ptr.get());
}

template<class T>
T*
Entity::set_component(std::unique_ptr<Component> component)
{
    static_assert(std::is_base_of<Component, T>(), "Require Component Subclass");

    auto& ptr = components_[std::type_index(typeid(T))];
    ptr = std::move(component);

    return static_cast<T*>(ptr.get());
}

template<class T>
T*
Entity::get_component()
{
    static_assert(std::is_base_of<Component, T>(), "Require Component Subclass");

    if (!has_component<T>()) {
        return nullptr;
    }

    auto& ptr = components_[std::type_index(typeid(T))];
    return static_cast<T*>(ptr.get());
}
