#include <algorithm>
#include <tuple>

#include "health_bar.hpp"
#include "utils.hpp"

HealthBar::HealthBar() :
    percent_filled_(1.f)
{
    set_solid(false);
}

void
HealthBar::set_filled(float percent_filled)
{
    percent_filled_ = std::max(0.f, std::min(1.f, percent_filled));
}

void
HealthBar::render()
{
    outline_.setOrigin(get_extents());
    outline_.setPosition(util::pixelate(get_position()));
    outline_.setSize(get_dimensions());
    outline_.setFillColor(sf::Color::White);
    float min_dimension = std::min(get_dimensions().x, get_dimensions().y);
    outline_.setOutlineThickness(min_dimension * OUTLINE_THICKNESS);
    outline_.setOutlineColor(sf::Color::Red);

    filled_.setOrigin({0.f, 0.f}); // outline is positioned via min corner to align with outline
    filled_.setPosition(util::pixelate(outline_.getPosition() - outline_.getOrigin()));
    filled_.setSize({get_dimensions().x * percent_filled_, get_dimensions().y});
    filled_.setFillColor(sf::Color::Red);

    clear_renderings();
    add_rendering(&outline_);
    add_rendering(&filled_);
}
