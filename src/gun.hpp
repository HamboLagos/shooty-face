#pragma once

#include <memory>

#include "entity.hpp"
#include "projectile.hpp"

/** \brief Gun is a collection of Projectiles which belongs to an operator. */
class Gun : public GraphicalEntity
{
public:
    using Magazine = std::vector<std::unique_ptr<Projectile>>;

    Gun(const Entity& the_operator);
    virtual ~Gun() = default;

    inline void set_ammunition(std::unique_ptr<Ammunition> ammo)
    { ammunition_ = std::move(ammo); }
    inline Ammunition* get_ammunition() const { return ammunition_.get(); }

    inline Magazine& get_magazine() { return magazine_; }

    /** \brief Fire a projectile of the set ammunition type at the given target.
     *
     * Automatic vs. Semiautomatic operation is determined by the ammunition type. */
    void fire(sf::Vector2f target);

    /** \brief Prepare for the next volley.
     *
     * Automatic vs. Semiautomatic operation is determined by the ammunition type. */
    void reload();

    void update(sf::Time elapsed) override;
    void render() override;

private:
    const Entity& operator_; ///< The entity operating this gun

    std::unique_ptr<Ammunition> ammunition_; ///< Ammunition type used for next fire()
    Magazine magazine_; ///< The projectiles this gun tracks (fired and unfired)
};
