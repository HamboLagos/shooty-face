#include "barrier.hpp"
#include "utils.hpp"

Barrier::Barrier()
{
    auto* graphics = add_component<Graphics>();
    graphics->on_render(std::bind(&Barrier::render, this));
}

const Graphics::Renderings
Barrier::render()
{
    graphic_.setSize(get_dimensions());
    graphic_.setOrigin(get_extents());
    graphic_.setPosition(util::pixelate(get_position()));
    graphic_.setFillColor(sf::Color::Black);

    Graphics::Renderings renderings;
    renderings.reserve(1);
    renderings.push_back(&graphic_);

    return std::move(renderings);
}
