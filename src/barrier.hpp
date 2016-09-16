#pragma once

#include <SFML/Graphics/RectangleShape.hpp>

#include "entity.hpp"
#include "graphics.hpp"

class Barrier : public Entity
{
public:
    Barrier();

    virtual ~Barrier() = default;

    void update(sf::Time elapsed) override { }

    const Graphics::Renderings render();

private:
    sf::RectangleShape graphic_;
};
