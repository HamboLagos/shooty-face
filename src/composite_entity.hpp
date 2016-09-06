#pragma once

#include <SFML/System/Time.hpp>

template <class T>
class CompositeEntity
{
public:
    CompositeEntity() = default;
    virtual ~CompositeEntity() = default;

    /** \brief Delegates update to each of its elements. */
    virtual void update(sf::Time elapsed) = 0;

    /** \brief Returns a collection of renderables for each of its elements. */
    virtual std::vector<const sf::Drawable*> render() = 0;

    /** \brief Returns a collection of this composite's elements. */
    virtual std::vector<T*> get_elements() const = 0;
};
