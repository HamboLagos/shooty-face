#include "gun.hpp"
#include "bullet.hpp"

using Ammunition = Gun::Ammunition;

void
Gun::set_ammunition(Ammunition ammo)
{
    ammunition_ = ammo;
}

Ammunition
Gun::get_ammunition() const
{
    return ammunition_;
}

Projectile*
Gun::get_last_projectile() const
{
    if (magazine_.empty()) {
        return nullptr;
    }

    return magazine_.back();
}

void
Gun::fire(sf::Vector2f target)
{
    switch (ammunition_) {

    case Ammunition::Bullet: {
            Bullet* bullet = new Bullet(operator_.get_position(), target, BULLET_SPEED) ;
            bullet->set_dimensions({10.f, 10.f});
            bullet->fire();
            magazine_.push_back(bullet);
        } break;

    default:
        break;
    }
}

void
Gun::update(sf::Time elapsed)
{
    auto remove_start = std::remove_if(magazine_.begin(), magazine_.end(),
                                       [](const auto* projectile){ return projectile->is_dead(); });
    magazine_.erase(remove_start, magazine_.end());

    for(auto* projectile : magazine_) {
        projectile->update(elapsed);
    }
}

std::vector<const sf::Drawable*>
Gun::render()
{
    std::vector<const sf::Drawable*> renderings;
    renderings.reserve(magazine_.size());

    for(auto* projectile : magazine_) {
        renderings.push_back(&projectile->render());
    }

    return renderings;
}

std::vector<Projectile*>
Gun::get_elements() const
{
    return std::vector<Projectile*>(magazine_); // returning copy
}
