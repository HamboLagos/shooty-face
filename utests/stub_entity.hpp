#pragma once

#include <entity.hpp>
#include <SFML/Graphics/RectangleShape.hpp>

class StubEntity : public Entity{
    void update(sf::Time elapsed) override { return; }

    /// don't call, not safe to return temp as reference
    const sf::Drawable& render() override { static sf::RectangleShape graphic; return graphic; }
};
