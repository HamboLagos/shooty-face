#pragma once

#include <SFML/Graphics/Drawable.hpp>

/** \brief Graphics provides a common interface for things rendered to screen. */
class Graphics : public Component
{
public:
    using Renderings = std::vector<const sf::Drawable*>;

    Graphics() = default;
    virtual ~Graphics() = default;

    /** \brief Render this Graphics. */
    const Renderings render() { return on_render_(); };

    /** \brief Callback on render.
     *
     * Takes no parameters, returns a collection of the Drawables rendered for this Graphics. */
    void on_render(std::function<const Renderings(void)> on_render) { on_render_ = on_render; }

private:
    std::function<const Renderings(void)> on_render_;
};
