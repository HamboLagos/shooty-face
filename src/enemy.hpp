#pragma once

#include <SFML/Graphics/RectangleShape.hpp>

#include "entity.hpp"
#include "components/graphics.hpp"
#include "components/AI.hpp"
#include "AI/A_star.hpp"

/** \brief Enemies are the ilk of the shooty-face universe, pursued with extreme prejudice. */
class Enemy : public Entity, public Renderer
{
public:
    static constexpr float SPEED = 100.f;

    Enemy();
    virtual ~Enemy() = default;

    void prepare() override { get_component<AI>()->prepare(); }
    void refresh(sf::Time frame_length) override;
    sf::Time update(sf::Time elapsed) override;
    void flush() override;

    const Renderings render() override;

private:
    sf::RectangleShape graphic_;
};
