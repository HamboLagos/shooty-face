#pragma once

#include <SFML/Graphics/RectangleShape.hpp>

#include "entity.hpp"
#include "healthy.hpp"

/** \brief Enemies are the ilk of the shooty-face universe, pursued with extreme prejudice. */
class Enemy : public GraphicalEntity, public Healthy
{
public:
    Enemy() = default;
    virtual ~Enemy() = default;

    void update(sf::Time elapsed) override;
    void render() override;

    void on_death() override;

private:
    sf::RectangleShape graphic_;
};
