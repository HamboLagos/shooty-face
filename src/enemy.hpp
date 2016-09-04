#pragma once

#include <SFML/Graphics/RectangleShape.hpp>

#include "entity.hpp"

class Enemy : public Entity
{
public:
    Enemy() = default;

    void update(sf::Time elapsed) override;
    const sf::Drawable& render() override;

private:
    sf::RectangleShape graphic_;
};
