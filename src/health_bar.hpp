#pragma once

#include <SFML/Graphics/RectangleShape.hpp>

#include "entity.hpp"

/** \brief Health Bar is a graphical representation of the health of a given entity. */
class HealthBar : public GraphicalEntity
{
static constexpr float OUTLINE_THICKNESS = -0.2f;

public:
    HealthBar();
    virtual ~HealthBar() = default;

    /** \brief Set the filled percentage of this health bar.
     *
     * Values outside the range [0.f, 1.f] are coerced to the nearest in-range value.
     *
     * \param[in] percent_filled Percentage of bar filled. */
    void set_filled(float percent_filled);
    inline float get_filled() const { return percent_filled_; } ///< Test Method

    /** \brief Update has no meaning in this context. */
    void update(sf::Time elapsed) override { return; }

    void render() override;

private:
    float percent_filled_;

    sf::RectangleShape outline_;
    sf::RectangleShape filled_;
};
