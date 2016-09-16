#pragma once

#include <SFML/System/Clock.hpp>

/** \brief Clock interface.
 *
 * Allows mocked time to be injected during tests. */
class ClockIF
{
public:
    ClockIF() = default;
    virtual ~ClockIF() = default;

    virtual sf::Time getElapsedTime() const = 0;
    virtual sf::Time restart() = 0;
};

/** \brief Default Clock implementation.
 *
 * Delegates to sf::Clock.*/
class Clock : public ClockIF
{
public:
    Clock() = default;
    virtual ~Clock() = default;

    sf::Time getElapsedTime() const { return clock_.getElapsedTime(); }
    sf::Time restart() { return clock_.restart(); }

private:
    sf::Clock clock_;
};
