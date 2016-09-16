#pragma once

#include <memory>
#include <typeindex>
#include <unordered_map>

#include <SFML/System/Time.hpp>

#include "components/component.hpp"

/** \brief Entity is a base for things which have dimension. */
class Entity {
public:
    using ComponentMap = std::unordered_map<std::type_index, std::unique_ptr<Component>>;

    Entity() :
        is_alive_(true)
    { }

    virtual ~Entity() = default;

    /** \brief Checks if this entity has the given component type. */
    template<class T> bool has_component();

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
    template<class T> T* get_component();

    /** \brief Update this entity.
     *
     * Typically called once per main-loop tick.
     *
     * \param[in] dt Time delta to perform update over, decouples game logic from framerate. */
    virtual void update(sf::Time elapsed) = 0;

    inline void set_alive(bool is_alive) { is_alive_ = is_alive; }
    inline void kill() { is_alive_ = false; }
    inline bool is_alive() const { return is_alive_; }
    inline bool is_dead() const { return !is_alive_; }

private:
    ComponentMap components_; ///< Map of the components for this entity

    bool is_alive_; ///< "Dead" entities are eventually removed from the game
};

#include "entity.inl"
