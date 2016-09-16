#include "gun.hpp"
#include "bullet.hpp"

Gun::Gun(const Entity& the_operator) :
    operator_(the_operator)
{ }

void
Gun::fire(sf::Vector2f target)
{
    auto projectile = ammunition_->create_projectile();
    if (projectile) {
        projectile->set_position(operator_.get_position());
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

void
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
}

const Graphics::Renderings
Gun::render()
{
    Graphics::Renderings renderings;
    renderings.reserve(magazine_.size());

    for(auto& projectile : magazine_) {
        renderings.push_back(projectile->render());
    }

    return renderings;
}
