#pragma once

#include <memory>

#include "clock.hpp"

/** \brief Simple class for rate-limiting an event.
 *
 * The clock is reset via renew(). Use check() to monitor the rate. */
class RateLimitIF
{
public:
    RateLimitIF() = default;
    virtual ~RateLimitIF() = default;

    /** \brief Checks the time limit for this RateLimit.
     *
     * \return true iff 1/rate seconds have passed since most recent renew()/construction. */
    virtual bool check() const = 0;

    /** \brief Restarts the internal timer. */
    virtual void renew() = 0;
};

class RateLimit : public RateLimitIF
{
friend class TestableRateLimit;

public:
    /** \brief Construct a rate limiter with the given rate.
     *
     * The clock is started on construction, and is restarted via renew(). */
    RateLimit(float rate) :
        RateLimit(rate, std::make_unique<Clock>())
    { }
    virtual ~RateLimit() = default;

    inline bool check() const override { return clock_->getElapsedTime().asSeconds() >= 1.f/rate_; }

    inline void renew() override { clock_->restart(); }

private:
    RateLimit(float rate, std::unique_ptr<ClockIF> clock) :
        rate_(rate),
        clock_(std::move(clock))
    { }

    const float rate_;
    std::unique_ptr<ClockIF> clock_;
};
