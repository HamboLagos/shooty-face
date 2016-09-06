#pragma once

#include <SFML/Graphics/RectangleShape.hpp>

#include "entity.hpp"
#include "healthy.hpp"

class Enemy : public Entity, public Healthy
{
public:
    Enemy() = default;

    void update(sf::Time elapsed) override;
    const sf::Drawable& render() override;

    void on_death() override;

private:
    sf::RectangleShape graphic_;
};
