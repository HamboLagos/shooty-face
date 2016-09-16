#pragma once

#include <SFML/Graphics/RectangleShape.hpp>

#include "entity.hpp"
#include "graphics.hpp"

/** \brief Enemies are the ilk of the shooty-face universe, pursued with extreme prejudice. */
class Enemy : public Entity
{
static constexpr float SLOW_SPEED = 50.f;
static constexpr float FAST_SPEED = 100.f;

static constexpr float WIDTH = 60.f;
static constexpr float HEIGHT = 60.f;

public:
    Enemy();
    virtual ~Enemy() = default;

    void update(sf::Time elapsed) override;

    const Graphics::Renderings render();

    void on_death();

    void set_fast() { is_fast_ = true; set_dimensions(sf::Vector2f(WIDTH, HEIGHT)); }
    void set_slow() { is_fast_ = false; set_dimensions(sf::Vector2f(WIDTH, HEIGHT)/2.f); }

private:
    sf::RectangleShape graphic_;

    sf::Vector2f destination_;
    bool is_fast_;
};
