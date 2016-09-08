#pragma once

#include <SFML/Graphics/RectangleShape.hpp>

#include "entity.hpp"
#include "utils.hpp"

class Barrier : public GraphicalEntity
{
public:
    Barrier() = default;
    virtual ~Barrier() = default;

    void update(sf::Time elapsed) override { }
    void render() override;

private:
    sf::RectangleShape graphic_;
};
