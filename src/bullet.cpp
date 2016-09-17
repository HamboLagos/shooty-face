#include "bullet.hpp"

#include "collision.hpp"
#include "components/health.hpp"
#include "components/physics.hpp"
#include "game.hpp"
#include "utils.hpp"

Bullet::Bullet()
{
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

void
Bullet::update(sf::Time elapsed)
{
    float dt = elapsed.asSeconds();
    auto box = get_component<Physics>()->get_box(dt);

    for(const auto& entity : Game::instance().entities()) {
        if (!Collision::sanity_check(*this, *entity)) {
            continue;
        }

        auto entity_box = entity->get_component<Physics>()->get_box();

        if (Collision::broad_test(box, entity_box))
        {
            float percent_safe = Collision::narrow_test(box, entity_box);
            if (percent_safe < 1.f) {
                if (entity->has_component<Health>()) {
                    entity->get_component<Health>()->damage(DAMAGE);
                }

                get_component<Physics>()->update(dt * percent_safe);
                kill();
            }
        }
    }

    if (is_alive()) {
        get_component<Physics>()->update(dt);
    }
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
