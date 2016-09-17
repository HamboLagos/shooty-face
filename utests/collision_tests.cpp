#include <gtest.h>
#include <gmock.h>

#include "collision.hpp"
#include "components/physics.hpp"
#include "mocks/entity_mock.hpp"

using namespace testing;

class TestableCollision : public Test { };

class SanityCheck : public TestableCollision { };

TEST_F(SanityCheck, SelfCollisionIsAlwaysInvalid)
{
    EntityMock first; first.add_component<Physics>();

    EXPECT_FALSE(Collision::sanity_check(first, first));
}

TEST_F(SanityCheck, NoPhysicsComponent)
{
    EntityMock first;
    EntityMock second;
    EXPECT_FALSE(Collision::sanity_check(first, second));

    first.add_component<Physics>();
    EXPECT_FALSE(Collision::sanity_check(first, second));

    first.set_component<Physics>(nullptr);
    second.add_component<Physics>();
    EXPECT_FALSE(Collision::sanity_check(first, second));
}

TEST_F(SanityCheck, PassableEntities)
{
    EntityMock first; first.add_component<Physics>()->set_solid(false);
    EntityMock second; second.add_component<Physics>()->set_solid(false);
    EXPECT_FALSE(Collision::sanity_check(first, second));

    first.get_component<Physics>()->set_solid(true);
    second.get_component<Physics>()->set_solid(false);
    EXPECT_FALSE(Collision::sanity_check(first, second));

    first.get_component<Physics>()->set_solid(false);
    second.get_component<Physics>()->set_solid(true);
    EXPECT_FALSE(Collision::sanity_check(first, second));
}

TEST_F(SanityCheck, PassesCheck)
{
    EntityMock first; first.add_component<Physics>();
    EntityMock second; second.add_component<Physics>();
    EXPECT_TRUE(Collision::sanity_check(first, second));
}

class BroadAndNarrowTest : public TestableCollision
{
protected:
    sf::Vector2f first_position_;
    sf::Vector2f first_dimensions_;
    sf::Vector2f first_trajectory_;

    sf::Vector2f second_position_;
    sf::Vector2f second_dimensions_;
    sf::Vector2f second_trajectory_;

    AABB first() { return AABB(first_position_, first_dimensions_, first_trajectory_); }
    AABB second() { return AABB(second_position_, second_dimensions_, second_trajectory_); }
};

TEST_F(BroadAndNarrowTest, AlongXAxis)
{
    first_position_ = {10.f, 0.f};
    first_dimensions_ = {2.f, 2.f};
    first_trajectory_ = {0.f, 0.f};

    second_position_ = {10.f, 0.f};
    second_dimensions_ = {2.f, 2.f};
    second_trajectory_ = {0.f, 0.f};

    // separated across the x-axis: 11 22
    second_position_.x = 13.f;
    EXPECT_FALSE(Collision::broad_test(first(), second()));
    EXPECT_FLOAT_EQ(1.f, Collision::narrow_test(first(), second()));

    // adjacent across the x-axis: 1122
    second_position_.x = 12.f;
    EXPECT_FALSE(Collision::broad_test(first(), second()));
    EXPECT_FLOAT_EQ(1.f, Collision::narrow_test(first(), second()));

    // intersecting on the x-axis: 1212
    second_position_.x = 11.f;
    EXPECT_TRUE(Collision::broad_test(first(), second()));
    EXPECT_FLOAT_EQ(0.f, Collision::narrow_test(first(), second()));

    // again, but use an expanded state space
    first_position_.x -= 1.f;
    first_trajectory_.x = 1.f;

    second_position_.x += 1.f;
    second_trajectory_.x = -1.f;
    EXPECT_TRUE(Collision::broad_test(first(), second()));
    EXPECT_FLOAT_EQ(0.5f, Collision::narrow_test(first(), second()));

    // last case, broad phase passes but narrow fails
    // second zooms past first, missing collision
    second_position_.y += 10.f;
    second_trajectory_.y = -100.f;
    EXPECT_TRUE(Collision::broad_test(first(), second()));
    EXPECT_FLOAT_EQ(1.f, Collision::narrow_test(first(), second()));
}

TEST_F(BroadAndNarrowTest, AlongYAxis)
{
    first_position_ = {0.f, 10.f};
    first_dimensions_ = {2.f, 2.f};
    first_trajectory_ = {0.f, 0.f};

    second_position_ = {0.f, 10.f};
    second_dimensions_ = {2.f, 2.f};
    second_trajectory_ = {0.f, 0.f};

    // separated across the x-axis: 11 22
    second_position_.y = 13.f;
    EXPECT_FALSE(Collision::broad_test(first(), second()));
    EXPECT_FLOAT_EQ(1.f, Collision::narrow_test(first(), second()));

    // adjacent across the x-axis: 1122
    second_position_.y = 12.f;
    EXPECT_FALSE(Collision::broad_test(first(), second()));
    EXPECT_FLOAT_EQ(1.f, Collision::narrow_test(first(), second()));

    // intersecting on the x-axis: 1212
    second_position_.y = 11.f;
    EXPECT_TRUE(Collision::broad_test(first(), second()));
    EXPECT_FLOAT_EQ(0.f, Collision::narrow_test(first(), second()));

    // again, but use an expanded state space
    first_position_.y -= 1.f;
    first_trajectory_.y = 1.f;

    second_position_.y += 1.f;
    second_trajectory_.y = -1.f;
    EXPECT_TRUE(Collision::broad_test(first(), second()));
    EXPECT_FLOAT_EQ(0.5f, Collision::narrow_test(first(), second()));

    // last case, broad phase passes but narrow fails
    // second zooms past first, missing collision
    second_position_.x += 10.f;
    second_trajectory_.x = -100.f;
    EXPECT_TRUE(Collision::broad_test(first(), second()));
    EXPECT_FLOAT_EQ(1.f, Collision::narrow_test(first(), second()));
}

TEST_F(BroadAndNarrowTest, FirstEncapsulatesSecond)
{
    first_position_ = {10.f, 10.f};
    first_dimensions_ = {2.f, 2.f};
    first_trajectory_ = {0.f, 0.f};

    second_position_ = {10.f, 10.f};
    second_dimensions_ = {1.f, 1.f};
    second_trajectory_ = {0.f, 0.f};

    EXPECT_TRUE(Collision::broad_test(first(), second()));
    EXPECT_FLOAT_EQ(0.f, Collision::narrow_test(first(), second()));

    // shrink first and second down to points
    first_dimensions_ = {0.f, 0.f};
    EXPECT_TRUE(Collision::broad_test(first(), second()));
    EXPECT_FLOAT_EQ(0.f, Collision::narrow_test(first(), second()));

    first_dimensions_ = {2.f, 2.f};
    second_dimensions_ = {0.f, 0.f};
    EXPECT_TRUE(Collision::broad_test(first(), second()));
    EXPECT_FLOAT_EQ(0.f, Collision::narrow_test(first(), second()));

    second_dimensions_ = {1.f, 1.f};

    // again, but use an expanded state space
    first_position_.y -= 1.f;
    first_trajectory_.y = 1.f;

    second_position_.x += 1.f;
    second_trajectory_.x = -1.f;
    EXPECT_TRUE(Collision::broad_test(first(), second()));
    EXPECT_FLOAT_EQ(0.f, Collision::narrow_test(first(), second()));

}

class NarrowTest : public BroadAndNarrowTest { };

TEST_F(NarrowTest, AtVaryingXTrajectories)
{
    first_position_ = {11.f, 11.f};
    first_dimensions_ = {2.f, 2.f};
    first_trajectory_ = {0.f, 0.f};

    second_position_ = {15.f, 11.f};
    second_dimensions_ = {2.f, 2.f};
    second_trajectory_ = {0.f, 0.f};

    EXPECT_FLOAT_EQ(1.f, Collision::narrow_test(first(), second()));

    first_trajectory_.x = 4.f;
    EXPECT_FLOAT_EQ(0.5f, Collision::narrow_test(first(), second()));

    second_trajectory_.x = -4.f;
    EXPECT_FLOAT_EQ(0.25f, Collision::narrow_test(first(), second()));

    second_trajectory_.x = 2.f;
    EXPECT_FLOAT_EQ(1.f, Collision::narrow_test(first(), second()));

    // mirror across the y-axis
    first_position_ = {-11.f, 11.f};
    first_dimensions_ = {2.f, 2.f};
    first_trajectory_ = {0.f, 0.f};

    second_position_ = {-15.f, 11.f};
    second_dimensions_ = {2.f, 2.f};
    second_trajectory_ = {0.f, 0.f};

    EXPECT_FLOAT_EQ(1.f, Collision::narrow_test(first(), second()));

    first_trajectory_.x = -4.f;
    EXPECT_FLOAT_EQ(0.5f, Collision::narrow_test(first(), second()));

    second_trajectory_.x = 4.f;
    EXPECT_FLOAT_EQ(0.25f, Collision::narrow_test(first(), second()));

    second_trajectory_.x = -2.f;
    EXPECT_FLOAT_EQ(1.f, Collision::narrow_test(first(), second()));
}

TEST_F(NarrowTest, AtVaryingYTrajectories)
{
    first_position_ = {11.f, 11.f};
    first_dimensions_ = {2.f, 2.f};
    first_trajectory_ = {0.f, 0.f};

    second_position_ = {11.f, 15.f};
    second_dimensions_ = {2.f, 2.f};
    second_trajectory_ = {0.f, 0.f};

    EXPECT_FLOAT_EQ(1.f, Collision::narrow_test(first(), second()));

    first_trajectory_.y = 4.f;
    EXPECT_FLOAT_EQ(0.5f, Collision::narrow_test(first(), second()));

    second_trajectory_.y = -4.f;
    EXPECT_FLOAT_EQ(0.25f, Collision::narrow_test(first(), second()));

    second_trajectory_.y = 2.f;
    EXPECT_FLOAT_EQ(1.f, Collision::narrow_test(first(), second()));

    // mirror across the x-axis
    first_position_ = {11.f, -11.f};
    first_dimensions_ = {2.f, 2.f};
    first_trajectory_ = {0.f, 0.f};

    second_position_ = {11.f, -15.f};
    second_dimensions_ = {2.f, 2.f};
    second_trajectory_ = {0.f, 0.f};

    EXPECT_FLOAT_EQ(1.f, Collision::narrow_test(first(), second()));

    first_trajectory_.y = -4.f;
    EXPECT_FLOAT_EQ(0.5f, Collision::narrow_test(first(), second()));

    second_trajectory_.y = 4.f;
    EXPECT_FLOAT_EQ(0.25f, Collision::narrow_test(first(), second()));

    second_trajectory_.y = -2.f;
    EXPECT_FLOAT_EQ(1.f, Collision::narrow_test(first(), second()));
}

class Penetration : public BroadAndNarrowTest
{
protected:
    sf::Vector2f penetration_;
};

TEST_F(Penetration, CalculateTheMinimumPenetrationVector)
{
    first_position_ = {10.f, 10.f};
    first_dimensions_ = {2.f, 2.f};
    first_trajectory_ = {0.f, 0.f};

    second_position_ = {10.f, 10.f};
    second_dimensions_ = {2.f, 2.f};
    second_trajectory_ = {0.f, 0.f};

    penetration_ = {0.f, 0.f};

    // intersecting on the x-axis: 1212
    second_position_.x = 11.f;
    penetration_ = {1.f, 0.f};

    EXPECT_TRUE(Collision::broad_test(first(), second()));
    EXPECT_FLOAT_EQ(0.f, Collision::narrow_test(first(), second()));
    EXPECT_EQ(penetration_, Collision::get_penetration(first(), second()));

    // intersecting on the x-axis: 2121
    second_position_.x = 9.f;
    penetration_ = {-1.f, 0.f};

    EXPECT_TRUE(Collision::broad_test(first(), second()));
    EXPECT_FLOAT_EQ(0.f, Collision::narrow_test(first(), second()));
    EXPECT_EQ(penetration_, Collision::get_penetration(first(), second()));

    first_position_ = {10.f, 10.f};
    second_position_ = {10.f, 10.f};
    penetration_ = {0.f, 0.f};

    // intersecting on the y-axis
    second_position_.y = 9.f;
    penetration_ = {0.f, -1.f};

    EXPECT_TRUE(Collision::broad_test(first(), second()));
    EXPECT_FLOAT_EQ(0.f, Collision::narrow_test(first(), second()));
    EXPECT_EQ(penetration_, Collision::get_penetration(first(), second()));

    second_position_.y = 11.f;
    penetration_ = {0.f, 1.f};

    EXPECT_TRUE(Collision::broad_test(first(), second()));
    EXPECT_FLOAT_EQ(0.f, Collision::narrow_test(first(), second()));
    EXPECT_EQ(penetration_, Collision::get_penetration(first(), second()));
}
