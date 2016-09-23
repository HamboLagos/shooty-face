#pragma once

#include <memory>
#include <typeindex>
#include <unordered_map>

#include <SFML/System/Time.hpp>

#include "components/component.hpp"

/** \brief An Entitiy is a container class for components. */
class Entity {
public:
    using ComponentMap = std::unordered_map<std::type_index, std::unique_ptr<Component>>;

    Entity() :
        is_alive_(true)
    { }
    virtual ~Entity() = default;

    /** \brief Called once before update() loop.
     *
     * Allows entities to perform first time setup for this frame.
     *
     * Not all entities will require setup, so the default implementation is empty.
     *
     * \param[in] frame_length The length of the current frame. */
    virtual void refresh(sf::Time frame_length) { return; }

    /** \brief Update this entity.
     *
     * Called once (or more) per main loop tick.
     *
     * Entity should return the time delta used during this update calculation. The main loop will
     * attempt to call update multipe times on this entity until it has used up the entire time
     * delta for this frame. This simplifies the collision logic, allowing the entity to update
     * until the nearest collision, then be to be called again to find the next nearest collision,
     * and so on. This also decouples the retry attempts for an update for a given entity from the
     * entity itself, so performance can be tweaked at a global scale.
     *
     * If the Entity is finished with its update (even before it has used the entire time delta
     * allotted for this frame), it should return the elapsed value passed in.
     *
     * \param[in] elapsed Time delta to perform update over, decouples game logic from framerate.
     * \return Time delta used during this update calculation, or 0 if finished. */
    virtual sf::Time update(sf::Time elapsed) = 0;

    /** \brief Called once after update() loop.
     *
     * Allows entities to perform tear down for this frame.
     *
     * Not all entities will require tear down, so the default implementation is empty. */
    virtual void flush() { return; }

    /** \brief Checks if entity has the given component.
     *
     * \return true iff this entity has the given component set, and its not nullptr. */
    template<class T> bool has_component() const;

    /** \brief Default constructs a new component for this entity.
     *
     * Only valid for components which take a single constructor argument: A reference to the entity
     * its attached to (ie. the "this" object). If the component already exists, it is replaced
     * unambiguously with a newly constructed one.
     *
     * \return The newly constructed component. */
    template<class T> T* add_component();

    /** \brief Sets the given component type.
     *
     * Necessary when the component has non-trivial construction requirements. If the component
     * already exists, it is replaced unambiguously with the given component.
     *
     * \return The newly set component. */
    template<class T> T* set_component(std::unique_ptr<Component> component);

    /** \brief Retrieves the given component from this entity.
     *
     * Clients can check whether this entity has this component via has_component().
     *
     * \return The given component for this entity, or nullptr if it doesn't have one. */
    template<class T> T* get_component() const;

    inline void set_alive(bool is_alive) { is_alive_ = is_alive; }
    inline void kill() { is_alive_ = false; }
    inline bool is_alive() const { return is_alive_; }
    inline bool is_dead() const { return !is_alive_; }

private:
    ComponentMap components_; ///< Map of the components for this entity

    bool is_alive_; ///< "Dead" entities are eventually removed from the game
};

#include "entity.inl"
