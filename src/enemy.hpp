#pragma once

#include <SFML/Graphics/RectangleShape.hpp>

#include "entity.hpp"
#include "components/graphics.hpp"

/** \brief Enemies are the ilk of the shooty-face universe, pursued with extreme prejudice. */
class Enemy : public Entity, public Renderer
{
public:
    static constexpr float SPEED = 100.f;

    Enemy();
    virtual ~Enemy() = default;

    void update(sf::Time elapsed) override;
    const Renderings render() override;

private:
    sf::RectangleShape graphic_;
};
