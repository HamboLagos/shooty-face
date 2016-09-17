#pragma once

#include "collision.hpp"

class CollisionMock
{
public:
    CollisionMock(const CollisionMock&)  = delete;
    void operator=(const CollisionMock&) = delete;

    static CollisionMock& instance()
    {
        static CollisionMock mock;
        return mock;
    }

    MOCK_METHOD2(sanity_check, bool(const Entity&, const Entity&));
    MOCK_METHOD2(broad_test, bool(const AABB&, const AABB&));
    MOCK_METHOD2(narrow_test, float(const AABB&, const AABB&));
    MOCK_METHOD2(get_penetration, sf::Vector2f(const AABB&, const AABB&));

private:
    CollisionMock() = default;
};

bool
Collision::sanity_check(const Entity& first, const Entity& second)
{
    return CollisionMock::instance().sanity_check(first, second);
}

bool
Collision::broad_test(const AABB& first, const AABB& second)
{
    return CollisionMock::instance().broad_test(first, second);
}

float
Collision::narrow_test(const AABB& first, const AABB& second)
{
    return CollisionMock::instance().narrow_test(first, second);
}

sf::Vector2f
Collision::get_penetration(const AABB& first, const AABB& second)
{
    return CollisionMock::instance().get_penetration(first, second);
}
