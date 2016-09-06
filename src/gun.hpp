#pragma once

#include <SFML/Graphics/RectangleShape.hpp>

#include "bullet.hpp"

class Gun : public Entity
{
public:
    enum class Ammunition
    {
        Bullet
    };

    static constexpr float BULLET_SPEED = 100.f;

    Gun(const Entity& the_operator) :
        operator_(the_operator)
    { }

    /** \brief Set the ammunition type for this gun. */
    void set_ammunition(Ammunition type);
    Ammunition get_ammunition() const;

    Projectile* get_last_projectile() const;

    /** \brief Fire a projectile of the set ammunition type at the given target. */
    void fire(sf::Vector2f target);

    /** \brief Calls update on each of the currently animated bullets. */
    void update(sf::Time elapsed) override;

    /** \brief For the moment, does nothing. */
    const sf::Drawable& render() override;

private:
    sf::RectangleShape graphic_;

    const Entity& operator_;

    Ammunition ammunition_;
    std::vector<Projectile*> magazine_;
};
