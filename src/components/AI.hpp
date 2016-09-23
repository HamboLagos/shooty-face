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

    /** \brief Default implementation is empty.
     *
     * \sa Entity::refresh(). */
    virtual void refresh(sf::Time frame_length) { return; }

    /** \brief Perform AI control of this entity for the current frame.
     *
     * \sa Entity::update(). */
    virtual sf::Time update(sf::Time elapsed) = 0;

    /** \brief Default implementation is empty.
     *
     * \sa Entity::flush(). */
    virtual void flush()   { return; }
};
