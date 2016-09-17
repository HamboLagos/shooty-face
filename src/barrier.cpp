#include "barrier.hpp"

#include "components/physics.hpp"
#include "utils.hpp"

Barrier::Barrier()
{
    auto* physics = add_component<Physics>();
    physics->set_static(true);

    set_component<Graphics>(std::make_unique<Graphics>(*this, *this));
}

void
Barrier::update(sf::Time elapsed)
{ }

const Renderer::Renderings
Barrier::render()
{
    auto* physics = get_component<Physics>();

    graphic_.setSize(physics->get_dimensions());
    graphic_.setOrigin(physics->get_extents());
    graphic_.setPosition(util::pixelate(physics->get_position()));
    graphic_.setFillColor(sf::Color::Black);

    Renderings renderings;
    renderings.reserve(1);
    renderings.push_back(&graphic_);

    return std::move(renderings);
}
