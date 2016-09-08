#include "barrier.hpp"

void
Barrier::render()
{
    clear_renderings();

    graphic_.setSize(get_dimensions());
    graphic_.setOrigin(get_extents());
    graphic_.setPosition(pixelate(get_position()));
    graphic_.setFillColor(sf::Color::Black);

    add_rendering(&graphic_);
}
