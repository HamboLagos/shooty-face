#include "AABB.hpp"
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

    float x1 = position_.x - get_x_extent();
    float x2 = position_.x + get_x_extent();
    if (std::abs(x1) != std::abs(x2)) {
        near_corner.x = std::abs(x1) < std::abs(x2) ? x1 : x2;
    } else {
        near_corner.x = get_min_corner().x;
    }

    float y1 = position_.y - get_y_extent();
    float y2 = position_.y + get_y_extent();
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

    float x1 = position_.x - get_x_extent();
    float x2 = position_.x + get_x_extent();
    if (std::abs(x1) != std::abs(x2)) {
        far_corner.x = std::abs(x1) > std::abs(x2) ? x1 : x2;
    } else {
        far_corner.x = get_max_corner().x;
    }

    float y1 = position_.y - get_y_extent();
    float y2 = position_.y + get_y_extent();
    if (std::abs(y1) != std::abs(y2)) {
        far_corner.y = std::abs(y1) > std::abs(y2) ? y1 : y2;
    } else {
        far_corner.y = get_max_corner().y;
    }

    return far_corner;
}

AABB
AABB::state_space_for(const AABB& box) {
    sf::Vector2f abs_trajectory = {
        std::abs(box.get_x_trajectory()),
        std::abs(box.get_y_trajectory())
    };

    return AABB(box.position_ + box.trajectory_/2.f,
                box.dimensions_ + abs_trajectory);
}

AABB
AABB::minkowski_difference(const AABB& first, const AABB& second)
{
    auto position = first.get_position() - second.get_position();
    auto dimensions = first.dimensions_ + second.dimensions_;
    auto trajectory = first.trajectory_ - second.trajectory_;

    return AABB(position, dimensions, trajectory);
}
