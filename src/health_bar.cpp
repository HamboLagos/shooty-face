#include <algorithm>
#include <tuple>

#include "health_bar.hpp"
#include "utils.hpp"

HealthBar::HealthBar(sf::Vector2f dimensions) :
    dimensions_(dimensions)
{ }

void
HealthBar::set_filled(float percent_filled)
{
    percent_filled_ = std::min(1.f, std::max(0.f, percent_filled));
}

std::vector<const sf::Drawable*>
HealthBar::render(sf::Vector2f position)
{
    float min_dimension = std::min(dimensions_.x, dimensions_.y);
    sf::Vector2f filled_dimensions = {dimensions_.x * percent_filled_, dimensions_.y};

    outline_.setSize(dimensions_);
    outline_.setOrigin(dimensions_/2.f);
    outline_.setPosition(pixelate(position));
    outline_.setOutlineThickness(min_dimension * outline_thickness_);
    outline_.setOutlineColor(sf::Color::Red);
    outline_.setFillColor(sf::Color::White);

    // outline is positioned via min corner to easily align with outline
    filled_.setSize(filled_dimensions);
    filled_.setPosition(pixelate(outline_.getPosition() - outline_.getOrigin()));
    filled_.setFillColor(sf::Color::Red);

    // ordering is important, so filled is drawn over the outline
    std::vector<const sf::Drawable*> renderings;
    renderings.push_back(&outline_);
    renderings.push_back(&filled_);

    return renderings;
}
