#pragma once

#include <SFML/System/Time.hpp>

#include "component.hpp"

/** \brief AI is a virtual base class for all AI types. */
class AI : public Component
{
public:
    AI(Entity& entity) :
        Component(entity)
    { }
    virtual ~AI() = default;

    /** \brief Perform AI control of this entity for the current frame. */
    virtual sf::Time update(sf::Time elapsed) = 0;
};
