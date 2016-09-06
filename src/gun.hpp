#pragma once

#include <SFML/Graphics/RectangleShape.hpp>

#include "composite_entity.hpp"
#include "bullet.hpp"

class Gun : public CompositeEntity<Projectile>
{
public:
    enum class Ammunition
    {
        Bullet
    };

    static constexpr float BULLET_SPEED = 250.f;

    Gun(const Entity& the_operator) :
        operator_(the_operator)
    { }

    /** \brief Set the ammunition type for this gun. */
    void set_ammunition(Ammunition type);
    Ammunition get_ammunition() const;

    Projectile* get_last_projectile() const;

    /** \brief Fire a projectile of the set ammunition type at the given target. */
    void fire(sf::Vector2f target);

    void update(sf::Time elapsed) override;
    std::vector<const sf::Drawable*> render() override;
    std::vector<Projectile*> get_elements() const override;

private:
    const Entity& operator_;

    Ammunition ammunition_;
    std::vector<Projectile*> magazine_;
};
