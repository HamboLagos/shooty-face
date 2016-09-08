#include "gun.hpp"
#include "bullet.hpp"

Gun::Gun(const Entity& the_operator) :
    operator_(the_operator)
{
    set_solid(false);
}

void
Gun::fire(sf::Vector2f target)
{
    auto projectile = ammunition_->create_projectile();
    projectile->set_position(operator_.get_position());
    projectile->set_target(target);
    projectile->fire();
    magazine_.push_back(std::unique_ptr<Projectile>(projectile));
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

void
Gun::render()
{
    clear_renderings();

    for(auto& projectile : magazine_) {
        projectile->render();
        add_renderings(projectile->get_renderings());
    }
}
