#pragma once

#include <SFML/Graphics/RectangleShape.hpp>

#include "entity.hpp"
#include "components/graphics.hpp"

/** \brief Barriers are impassable Rectangle shapes. */
class Barrier : public Entity, public Renderer
{
public:
    Barrier();
    virtual ~Barrier() = default;

    sf::Time update(sf::Time elapsed) override;
    const Renderings render() override;

private:
    sf::RectangleShape graphic_;
};
