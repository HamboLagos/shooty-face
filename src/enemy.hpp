#pragma once

#include <SFML/Graphics/RectangleShape.hpp>

#include "entity.hpp"

class Enemy : public Entity
{
public:
    Enemy();

    void damage(float damage);

    void update(sf::Time elapsed) override;
    const sf::Drawable& render() override;

private:
    sf::RectangleShape graphic_;

    float health_;
};
