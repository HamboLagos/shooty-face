#pragma once

#include <SFML/Graphics/RectangleShape.hpp>

#include "entity.hpp"
#include "healthy.hpp"

/** \brief Enemies are the ilk of the shooty-face universe, pursued with extreme prejudice. */
class Enemy : public GraphicalEntity, public Healthy
{
static constexpr float SLOW_SPEED = 50.f;
static constexpr float FAST_SPEED = 100.f;

static constexpr float WIDTH = 60.f;
static constexpr float HEIGHT = 60.f;

public:
    Enemy();
    virtual ~Enemy() = default;

    inline void set_destination(sf::Vector2f destination) { destination_ = destination; }

    void update(sf::Time elapsed) override;
    void render() override;

    void on_death() override;

    void set_fast() { is_fast_ = true; set_dimensions(sf::Vector2f(WIDTH, HEIGHT)); }
    void set_slow() { is_fast_ = false; set_dimensions(sf::Vector2f(WIDTH, HEIGHT)/2.f); }

private:
    sf::RectangleShape graphic_;

    sf::Vector2f destination_;
    bool is_fast_;
};
