#include <cmath>
#include <limits>

#include "collision.hpp"
#include "components/physics.hpp"
#include "utils.hpp"

const sf::Vector2f Collision::ORIGIN = sf::Vector2f(0.f, 0.f);

bool
Collision::sanity_check(const Entity& first, const Entity& second)
{
    if (&first == &second) {
        return false;
    }

    if (!first.has_component<Physics>() || !second.has_component<Physics>()) {
        return false;
    }

    if (first.get_component<Physics>()->is_passable() ||
        second.get_component<Physics>()->is_passable()) {
        return false;
    }

    return true;
}

bool
Collision::broad_test(const AABB& first, const AABB& second)
{
    auto mk_diff = AABB::minkowski_difference(AABB::state_space_for(first),
                                              AABB::state_space_for(second));
    return mk_diff.contains_point(ORIGIN);
}

float
Collision::narrow_test(const AABB& first, const AABB& second)
{
    // protect ourselves from impossible collisions
    if (!broad_test(first, second)) {
        return 1.f;
    }

    auto mk_diff = AABB::minkowski_difference(first, second);

    // colliding before trajectory applied
    if (mk_diff.contains_point(ORIGIN)) {
        return 0.f;
    }

    // Find the entry and exit time for x and y
    // using [noonat.github.io/intersect]
    float entry_x = -std::numeric_limits<float>::infinity();
    float exit_x = std::numeric_limits<float>::infinity();
    float entry_y = -std::numeric_limits<float>::infinity();
    float exit_y = std::numeric_limits<float>::infinity();

    if (mk_diff.get_trajectory().x != 0.f) {
        entry_x = mk_diff.get_near_corner().x / -mk_diff.get_trajectory().x;
        exit_x  = mk_diff.get_far_corner().x  / -mk_diff.get_trajectory().x;
    }

    if (mk_diff.get_trajectory().y != 0.f) {
        entry_y = mk_diff.get_near_corner().y / -mk_diff.get_trajectory().y;
        exit_y  = mk_diff.get_far_corner().y  / -mk_diff.get_trajectory().y;
    }

    if (entry_x > exit_y || entry_y > exit_x) {
        return 1.f;
    }

    float entry = std::max(entry_x, entry_y);
    float exit = std::min(exit_x, exit_y);

    if (entry >= 1.f || exit <= 0.f) {
        return 1.f;
    }

    return entry;
}

sf::Vector2f
Collision::get_penetration(const AABB& first, const AABB& second)
{
    auto mk_diff = AABB::minkowski_difference(first, second);
    auto near = mk_diff.get_near_corner();

    // if it's rigt on the edge, return a small value
    float small_value = 0.01f;
    if (near.x == 0.f) {
        return -util::direction({mk_diff.get_far_corner().x, 0.f}) * small_value;
    }
    if (near.y == 0.f) {
        return -util::direction({0.f, mk_diff.get_far_corner().y}) * small_value;
    }

    // if penetration is nonzero, return it
    if (std::abs(near.x) <= std::abs(near.y)) {
        return sf::Vector2f(near.x, 0.f);
    }

    return sf::Vector2f(0.f, near.y);
}
