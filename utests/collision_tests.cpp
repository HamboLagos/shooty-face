#include <gtest.h>
#include <gmock.h>

#include <collision.hpp>
#include <AABB.hpp>

using namespace testing;

class TestableCollision : public Test
{
protected:
    Collision sut;
};

class DualStaticEntities : public TestableCollision
{
protected:
    sf::Vector2f one_origin;
    sf::Vector2f one_dimensions;

    sf::Vector2f other_origin;
    sf::Vector2f other_dimensions;

    AABB one() { return AABB(one_origin, one_dimensions); }
    AABB other() { return AABB(other_origin, other_dimensions); }
};

TEST_F(DualStaticEntities, CollideIfIntersecting)
{
    one_origin = {10.f, 10.f};
    one_dimensions = {2.f, 2.f};
    other_origin = {10.f, 10.f};
    other_dimensions = {2.f, 2.f};

    // separated across the x-axis: 11 22
    other_origin.x = 13.f;
    EXPECT_FALSE(sut.test(one(), other()));

    // adjacent across the x-axis: 1122
    other_origin.x = 12.f;
    EXPECT_FALSE(sut.test(one(), other()));

    // intersecting on the x-axis: 1212
    other_origin.x = 11.f;
    EXPECT_TRUE(sut.test(one(), other()));

    one_origin = {10.f, 10.f};
    one_dimensions = {2.f, 2.f};
    other_origin = {10.f, 10.f};
    other_dimensions = {2.f, 2.f};

    // separated across the y-axis
    other_origin.y = 13.f;
    EXPECT_FALSE(sut.test(one(), other()));

    // adjacent across the y-axis
    other_origin.y = 12.f;
    EXPECT_FALSE(sut.test(one(), other()));

    // intersecting on the y-axis
    other_origin.y = 11.f;
    EXPECT_TRUE(sut.test(one(), other()));

    one_origin = {10.f, 10.f};
    one_dimensions = {3.f, 4.f};
    other_origin = {10.f, 10.f};
    other_dimensions = {3.f, 4.f};

    // coincident on a corner
    other_origin = {13.f, 14.f};
    EXPECT_FALSE(sut.test(one(), other()));

    // intersection through a corner
    other_origin = {12.f, 13.f};
    EXPECT_TRUE(sut.test(one(), other()));

    one_origin = {10.f, 10.f};
    one_dimensions = {10.f, 10.f};
    other_origin = {12.f, 12.f};
    other_dimensions = {2.f, 2.f};

    // One fully encapsulates the other
    EXPECT_TRUE(sut.test(one(), other()));
}

TEST_F(DualStaticEntities, CalculateTheMinimumPenetrationVector)
{
    one_origin = {10.f, 10.f};
    one_dimensions = {2.f, 2.f};
    other_origin = {10.f, 10.f};
    other_dimensions = {2.f, 2.f};
    sf::Vector2f penetration = {0.f, 0.f};

    // intersecting on the x-axis: 1212
    other_origin.x = 11.f;
    penetration = {-1.f, 0.f};
    EXPECT_TRUE(sut.test(one(), other()));
    EXPECT_FLOAT_EQ(sut.get_penetration().x, penetration.x);
    EXPECT_FLOAT_EQ(sut.get_penetration().y, penetration.y);

    // intersecting on the x-axis: 2121
    other_origin.x = 9.f;
    penetration = {1.f, 0.f};
    EXPECT_TRUE(sut.test(one(), other()));
    EXPECT_FLOAT_EQ(sut.get_penetration().x, penetration.x);
    EXPECT_FLOAT_EQ(sut.get_penetration().y, penetration.y);

    one_origin = {10.f, 10.f};
    one_dimensions = {2.f, 2.f};
    other_origin = {10.f, 10.f};
    other_dimensions = {2.f, 2.f};
    penetration = {0.f, 0.f};

    // intersecting on the y-axis
    other_origin.y = 9.f;
    penetration = {0.f, 1.f};
    EXPECT_TRUE(sut.test(one(), other()));
    EXPECT_FLOAT_EQ(sut.get_penetration().x, penetration.x);
    EXPECT_FLOAT_EQ(sut.get_penetration().y, penetration.y);

    other_origin.y = 11.f;
    penetration = {0.f, -1.f};
    EXPECT_TRUE(sut.test(one(), other()));
    EXPECT_FLOAT_EQ(sut.get_penetration().x, penetration.x);
    EXPECT_FLOAT_EQ(sut.get_penetration().y, penetration.y);
}
