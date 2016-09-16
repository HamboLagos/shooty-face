#pragma once

class Entity; // forward declaration prevents recursive include

/** \brief Component is the base class of all Components in the Component-Entity system.
 *
 * Every Component *must* be associated with one, and only one, Entity. That entity is available to
 * sublcasses through the protected getter method.
 *
 * The Entity for a Component cannot be changed after construction. */
class Component
{
public:
    Component(Entity& entity) :
        entity_(entity)
    { }

    virtual ~Component() = default;

protected:
    Entity& get_entity() { return entity_; }

private:
    Entity& entity_; ///< The entity for this component
};


