#pragma once

#include "entity.hpp"

/* #include "health_bar.hpp" */

class Health : public Component
{
public:
    Health(float base_health = 100.f);

    virtual ~Health() = default;

    /** \brief Set health to an absolute value.
     *
     * Does not trigger any callbacks. Health is coerced to nearest value in range
     * [0.f, base_health].
     *
     * \param[in] health New health value. */
    void set_health(float health);

    /** \brief Returns the current health.
     *
     * Callbacks are called after health has been updated, so calling this from within a callback
     * will return the health post-update. */
    inline float get_health() const { return health_; };

    /* Graphical::Renderings render(sf::Vector2f position, float y_off); */

    /** \brief Reduces health by the given amount.
     *
     * Always triggers Health::on_damage(), and may trigger Health::on_death() if resulting health
     * is <= 0.
     *
     * Before callbacks, health is coerced to nearest value in range [0.f, base_health].
     *
     * \param[in] damage Health reduction amount. */
    void damage(float damage);

    /** \brief Increases health by the given amount.
     *
     * Always triggers Health::on_heal().
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
     * use std::bind to map to a member function. */
    inline void on_heal(std::function<void(float)> callback) { on_heal_ = callback; }

    /** \brief Callback on Health::damage().
     *
     * Callback is called after damaging effect is applied, and is passed the damage amount. Total
     * health, post-damage, can be queried through Health::get_health().
     *
     * use std::bind to map to a member function. */
    inline void on_damage(std::function<void(float)> callback) { on_damage_ = callback; }

    /** \brief Callback on health <= 0.
     *
     * Callback is called after any non-const function in Health:: is called which results in
     * a health balance <= 0. This is in addition to any other callbacks which may be triggered as
     * a result of the operation, such as through Health::damage().
     *
     * use std::bind to map to a member function. */
    void on_death(std::function<void(void)> callback) { on_death_ = callback; }

private:
    float base_health_;
    float health_;

    std::function<void(float)> on_heal_;
    std::function<void(float)> on_damage_;
    std::function<void(void)>  on_death_;

    /* HealthBar health_bar_; */
};
