#pragma once

#include <gmock.h>

#include "collision.hpp"

class CollisionMock
{
public:
    CollisionMock(const CollisionMock&)  = delete;
    void operator=(const CollisionMock&) = delete;

    static CollisionMock& instance()
    {
        using namespace testing;

        static NiceMock<CollisionMock> mock;
        return mock;
    }

    static void set_defaults()
    {
        using namespace testing;

        ON_CALL(instance(), sanity_check(_, _)).WillByDefault(Return(false));
        ON_CALL(instance(), broad_test(_, _)).WillByDefault(Return(false));
        ON_CALL(instance(), narrow_test(_, _)).WillByDefault(Return(1.f));
        ON_CALL(instance(), get_penetration(_, _)).WillByDefault(Return(sf::Vector2f(0.f, 0.f)));
    }

    static void clear_defaults()
    {
        using namespace testing;

        Mock::VerifyAndClear(&instance());
    }

    MOCK_METHOD2(sanity_check, bool(const Entity&, const Entity&));
    MOCK_METHOD2(broad_test, bool(const AABB&, const AABB&));
    MOCK_METHOD2(narrow_test, float(const AABB&, const AABB&));
    MOCK_METHOD2(get_penetration, sf::Vector2f(const AABB&, const AABB&));

private:
    friend class testing::NiceMock<CollisionMock>;
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
