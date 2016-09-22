#include "gun.hpp"

#include "components/physics.hpp"

Gun::Gun(const Entity& the_operator) :
    operator_(the_operator)
{
    set_component<Graphics>(std::make_unique<Graphics>(*this, *this));
}

void
Gun::fire(sf::Vector2f target)
{
    auto projectile = ammunition_->create_projectile();
    if (projectile) {
        auto position = operator_.get_component<Physics>()->get_position();
        projectile->get_component<Physics>()->set_position(position);
        projectile->set_target(target);
        projectile->fire();
        magazine_.emplace_back(projectile);
    }
}

void
Gun::reload()
{
    ammunition_->reload();
}

sf::Time
Gun::update(sf::Time elapsed)
{
    // remove all the dead projectiles
    magazine_.erase(
        std::remove_if(magazine_.begin(), magazine_.end(),
                       [](const auto& projectile){ return projectile->is_dead(); }),
        magazine_.end());

    // update the remaining live projectiles
    for(auto& projectile : magazine_) {
        projectile->update(elapsed);
    }

    return sf::Time::Zero;
}

const Graphics::Renderings
Gun::render()
{
    Graphics::Renderings renderings;
    renderings.reserve(magazine_.size());

    for(auto& projectile : magazine_) {
        const auto& projectile_renderings = projectile->render();
        renderings.insert(renderings.end(), projectile_renderings.begin(), projectile_renderings.end());
    }

    return std::move(renderings);
}
