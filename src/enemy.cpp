#include "enemy.hpp"

#include "AI/AI_enemy.hpp"
#include "components/health.hpp"
#include "components/physics.hpp"
#include "utils.hpp"

Enemy::Enemy()
{
    set_component<AI>(std::make_unique<AIEnemy>(*this));

    auto* physics = add_component<Physics>();
    physics->set_dimensions({40.f, 40.f});
    physics->set_move_speed(SPEED);

    auto* graphics = set_component<Graphics>(std::make_unique<Graphics>(*this, *this));

    add_component<Health>();
}

void
Enemy::refresh(sf::Time frame_length)
{
    get_component<AI>()->refresh(frame_length);
}

sf::Time
Enemy::update(sf::Time elapsed)
{
    return get_component<AI>()->update(elapsed);
}

void
Enemy::flush()
{
    get_component<Health>()->update();
}

const Graphics::Renderings
Enemy::render()
{
    auto* physics = get_component<Physics>();

    graphic_.setSize(physics->get_dimensions());
    graphic_.setOrigin(physics->get_extents());
    graphic_.setPosition(util::pixelate(physics->get_position()));
    graphic_.setFillColor(sf::Color::Green);
    graphic_.setOutlineThickness(-1.f);
    graphic_.setOutlineColor(sf::Color::Black);

    Renderings renderings;
    renderings.reserve(1);
    renderings.push_back(&graphic_);

    return std::move(renderings);
}
