#include "AABB.hpp"

AABB::AABB(sf::Vector2f position, sf::Vector2f dimensions, sf::Vector2f trajectory) :
    position_(position),
    dimensions_(dimensions),
    extents_(dimensions/2.f),
    trajectory_(trajectory)
{ }

sf::Vector2f
AABB::get_min_corner() const
{
    return position_ - get_extents();
}

sf::Vector2f
AABB::get_max_corner() const
{
    return position_ + get_extents();
}

sf::Vector2f
AABB::get_near_corner() const
{
    sf::Vector2f near_corner;

    float x1 = position_.x - extents_.x;
    float x2 = position_.x + extents_.x;

    if (std::abs(x1) != std::abs(x2)) {
        near_corner.x = std::abs(x1) < std::abs(x2) ? x1 : x2;
    } else {
        near_corner.x = get_min_corner().x;
    }

    float y1 = position_.y - extents_.y;
    float y2 = position_.y + extents_.y;

    if (std::abs(y1) != std::abs(y2)) {
        near_corner.y = std::abs(y1) < std::abs(y2) ? y1 : y2;
    } else {
        near_corner.y = get_min_corner().y;
    }

    return near_corner;
}

sf::Vector2f
AABB::get_far_corner() const
{
    sf::Vector2f far_corner;

    float x1 = position_.x - extents_.x;
    float x2 = position_.x + extents_.x;

    if (std::abs(x1) != std::abs(x2)) {
        far_corner.x = std::abs(x1) > std::abs(x2) ? x1 : x2;
    } else {
        far_corner.x = get_max_corner().x;
    }

    float y1 = position_.y - extents_.y;
    float y2 = position_.y + extents_.y;

    if (std::abs(y1) != std::abs(y2)) {
        far_corner.y = std::abs(y1) > std::abs(y2) ? y1 : y2;
    } else {
        far_corner.y = get_max_corner().y;
    }

    return far_corner;
}

bool
AABB::contains_point(sf::Vector2f point) const
{
    auto min = get_min_corner();
    auto max = get_max_corner();

    return
        min.x < point.x &&
        max.x > point.x &&
        min.y < point.y &&
        max.y > point.y;
}

AABB
AABB::state_space_for(const AABB& box) {

    auto abs_trajectory = sf::Vector2f(std::abs(box.trajectory_.x), std::abs(box.trajectory_.y));

    return AABB(box.position_ + box.trajectory_/2.f,
                box.dimensions_ + abs_trajectory);
}

AABB
AABB::minkowski_difference(const AABB& first, const AABB& second)
{
    auto position = first.position_ - second.position_;
    auto dimensions = first.dimensions_ + second.dimensions_;
    auto trajectory = first.trajectory_ - second.trajectory_;

    return AABB(position, dimensions, trajectory);
}
