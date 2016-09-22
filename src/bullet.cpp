#include "bullet.hpp"

#include "AI/AI_bullet.hpp"
#include "collision.hpp"
#include "components/AI.hpp"
#include "components/health.hpp"
#include "components/physics.hpp"
#include "game.hpp"
#include "utils.hpp"

Bullet::Bullet()
{
    set_component<AI>(std::make_unique<AIBullet>(*this));

    auto* physics = add_component<Physics>();
    physics->set_dimensions({10.f, 10.f});
    physics->set_static(true);
    physics->set_move_speed(SPEED);

    set_component<Graphics>(std::make_unique<Graphics>(*this, *this));
}

void
Bullet::fire()
{
    auto* physics = get_component<Physics>();
    auto trajectory = get_target() - physics->get_position();

    physics->set_velocity(util::direction(trajectory) * physics->get_move_speed());
    physics->set_static(false);
}

sf::Time
Bullet::update(sf::Time elapsed)
{
    return get_component<AI>()->update(elapsed);
}

const Renderer::Renderings
Bullet::render()
{
    auto* physics = get_component<Physics>();

    graphic_.setRadius(physics->get_extents().x);
    graphic_.setOrigin(physics->get_extents());
    graphic_.setPosition(util::pixelate(physics->get_position()));
    graphic_.setFillColor(sf::Color::Red);

    Renderings renderings;
    renderings.reserve(1);
    renderings.push_back(&graphic_);

    return std::move(renderings);
}

void
Bullet::apply_damage(const Entity& entity)
{
    if (entity.has_component<Health>()) {
        entity.get_component<Health>()->damage(DAMAGE);
    }
}

Projectile*
BulletAmmunition::create_projectile()
{
    if (!is_firing_ || firing_pin_->getElapsedTime().asSeconds() >= 1.f/FIRE_RATE) {
        is_firing_ = true;
        firing_pin_->restart();

        return new Bullet;
    }

    return nullptr;
}

void
BulletAmmunition::reload()
{
    is_firing_ = false;
}
