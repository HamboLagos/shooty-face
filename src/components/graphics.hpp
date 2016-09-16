#pragma once

#include <SFML/Graphics/Drawable.hpp>

#include "component.hpp"

/** \brief Interface for classes which handle rendering. */
class Renderer
{
public:
    using Renderings = std::vector<const sf::Drawable*>;

    Renderer() = default;
    virtual ~Renderer() = default;

    /** \brief Returns a collection of the renderings to draw to screen.
     *
     * Returned renderings should conform to the rules for sf::Draw(). */
    virtual const Renderings render() = 0;
};

/** \brief Entities which have a Graphics component draw things to the screen.
 *
 * The renderer, which handles the drawing, is typically the entity itself, but can be delegated to
 * a separate class for groups of entities which are rendered together. */
class Graphics : public Component
{
public:
    using Renderings = Renderer::Renderings;

    /** \param[in] entity The entity for this component.
     *  \param[in] renderer The delegated renderer, typically the same as entity. */
    Graphics(Entity& entity, Renderer& renderer) :
        Component(entity),
        renderer_(renderer)
    { }

    virtual ~Graphics() = default;

    /** \brief Delegates rendering to the renderer.
     *
     * The renderer is initialized on construction, and is usally the same as the entity this
     * Graphics Component is for. */
    const Renderings render() { return renderer_.render(); };

private:
    Renderer& renderer_;
};
