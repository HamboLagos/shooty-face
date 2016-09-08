#pragma once

#include <SFML/Graphics/Drawable.hpp>

/** \brief Graphical provides a common interface for things rendered to screen. */
class Graphical
{
public:
    using Renderings = std::vector<const sf::Drawable*>;

    Graphical() = default;
    virtual ~Graphical() = default;

    /** \brief Retrieve the collection of renderings for this Graphical.
     *
     * Clients should update the renderings via render() on each tick of the game loop before
     * calling this. */
    const Renderings& get_renderings() const { return renderings_; }

    /** \brief Render this Graphical.
     *
     * The Graphical class does not allocate storage space for graphics, because rendering types
     * vary by subclass. The processed renderings are retrieved via get_renderings(). */
    virtual void render() = 0;

protected:
    inline void clear_renderings() { renderings_.clear(); }
    inline void reserve_renderings(int space) { renderings_.reserve(space); }
    inline void add_rendering(const sf::Drawable* rendering) { renderings_.push_back(rendering); }
    inline void add_renderings(const Renderings& renderings)
    { renderings_.insert(renderings_.end(), renderings.begin(), renderings.end()); }

private:
    Renderings renderings_;
};
