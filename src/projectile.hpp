#pragma once

#include <SFML/Graphics.hpp>

#include "entity.hpp"

class Projectile : public Entity
{
public:
    Projectile() :
        graphic_()
    { }

    void update(sf::Time elapsed) override;
    const sf::Drawable& render() override;

private:
    sf::CircleShape graphic_;
};

