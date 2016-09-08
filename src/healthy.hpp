#pragma once

#include "health_bar.hpp"

class Healthy
{
public:
    Healthy(float base_health = 100.f);

    virtual ~Healthy() = default;

    /** \brief Set health to an absolute value.
     *
     * Does not trigger any callbacks. Health is coerced to nearest value in range
     * [0.f, base_health].
     *
     * \param[in] health New health value. */
    void set_health(float health);
    inline float get_health() const { return health_; };

    Graphical::Renderings render(sf::Vector2f position, float y_off);

    /** \brief Reduces health by the given amount.
     *
     * Always triggers Healthy::on_damage(), and may trigger Healthy::on_death() if resulting health
     * is <= 0.
     *
     * Before callbacks, health is coerced to nearest value in range [0.f, base_health].
     *
     * \param[in] damage Health reduction amount. */
    void damage(float damage);

    /** \brief Increases health by the given amount.
     *
     * Always triggers Healthy::on_heal().
     *
     * Before callbacks, health is coerced to nearest value in range [0.f, base_health].
     *
     * \param[in] heal Health increase amount. */
    void heal(float heal);

    /** \brief Callback on Health::heal().
     *
     * Callback is called after healing effect is applied, and is passed the healed amount. Total
     * health, post-heal, can be queried through Health::get_health().
     *
     * \param[in] heal Heal amount, same as passed to Health::heal(). */
    virtual void on_heal(float heal) { }

    /** \brief Callback on Health::damage().
     *
     * Callback is called after damaging effect is applied, and is passed the damage amount. Total
     * health, post-damage, can be queried through Health::get_health().
     *
     * \param[in] damage Damage amount, same as passed to Health::damage. */
    virtual void on_damage(float damage) { }

    /** \brief Callback on health <= 0.
     *
     * Callback is called after any non-const function in Health:: is called which results in
     * a health balance <= 0. This is in addition to any other callbacks which may be triggered as
     * a result of the operation, such as through Health::damage(). */
    virtual void on_death() { }

private:
    float base_health_;
    float health_;

    HealthBar health_bar_;
};
