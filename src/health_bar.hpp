#pragma once

#include <SFML/Graphics/RectangleShape.hpp>

class HealthBar
{
public:
    HealthBar(sf::Vector2f dimensions);

    /** \brief Set the filled percentage of this health bar.
     *
     * Values outside the range [0.f, 1.f] are coerced to the nearest in-range value.
     *
     * \param[in] percent_filled Percentage of bar filled. */
    void set_filled(float percent_filled);

    /// Test Method
    inline float get_filled() const { return percent_filled_; }

    /** \brief Render the health bar at the given position.
     *
     * \param[in] position Geometrically centered position for the health bar.
     * \return Collection of renderings, must be drawn in order. */
    std::vector<const sf::Drawable*> render(sf::Vector2f position);

private:
    float percent_filled_;

    sf::Vector2f dimensions_;
    sf::RectangleShape outline_;
    sf::RectangleShape filled_;

    static constexpr float outline_thickness_ = -0.2f;
};
