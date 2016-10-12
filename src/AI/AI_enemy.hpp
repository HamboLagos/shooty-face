#pragma once

#include "AI/A_star.hpp"
#include "components/AI.hpp"
#include "enemy.hpp"
#include "rate_limit.hpp"

/** \brief AI control for the enemy class.
 *
 * Moves towards the player until it is hugging them. Later on, we'll teach it how to deal damage
 * and shoot the player, and other nefarious things. */
class AIEnemy : public AI
{
friend class TestableAIEnemy;

    static const float ASTAR_REFRESH_RATE;

public:
    AIEnemy(Enemy& enemy) :
        AIEnemy(enemy, std::make_unique<RateLimit>(ASTAR_REFRESH_RATE))
    { }
    virtual ~AIEnemy() = default;

    void prepare() override;

    /** \brief Sets the physics state to track the player. */
    void refresh(sf::Time frame_length) override;

    /** \brief Updates the physics state until the nearest collision. */
    sf::Time update(sf::Time elapsed) override;

    inline Enemy& get_enemy() { return static_cast<Enemy&>(get_entity()); }

    inline const AStar::Path& get_path() const { return path_; }
    inline const int get_path_ndx() const { return path_ndx_; }

private:
    AIEnemy(Enemy& enemy, std::unique_ptr<RateLimitIF> refresh_rate) :
        AI(enemy),
        refresh_rate_(std::move(refresh_rate)),
        has_path_(false),
        fractional_move_(0.f),
        user_ndx_(user_count_)
    {
        ++user_count_;
    }

    std::unique_ptr<RateLimitIF> refresh_rate_; ///< limits (costly) path calculation

    bool has_path_;
    AStar::Path path_;
    int path_ndx_;

    float fractional_move_;

    static int user_count_;
    static int next_user_;
    int user_ndx_;
};
