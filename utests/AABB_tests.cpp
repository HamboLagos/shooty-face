#include <gtest.h>
#include <gmock.h>

#include "AABB.hpp"

using namespace testing;

class TestableAABB : public Test { };

class CornersAndContainsPoint : public TestableAABB
{
protected:
    static const sf::Vector2f dimensions_;
};
const sf::Vector2f CornersAndContainsPoint::dimensions_ = {2.f, 2.f};

TEST_F(CornersAndContainsPoint, Quadrant1)
{
    AABB sut({8.f, 8.f}, dimensions_);

    EXPECT_EQ(sf::Vector2f(7.f, 7.f), sut.get_min_corner());
    EXPECT_EQ(sf::Vector2f(9.f, 9.f), sut.get_max_corner());

    EXPECT_EQ(sf::Vector2f(7.f, 7.f), sut.get_near_corner());
    EXPECT_EQ(sf::Vector2f(9.f, 9.f), sut.get_far_corner());

    EXPECT_TRUE(sut.contains_point(sf::Vector2f(8.5f, 8.5f)));
    EXPECT_TRUE(sut.contains_point(sf::Vector2f(7.5f, 8.5f)));
    EXPECT_TRUE(sut.contains_point(sf::Vector2f(8.5f, 7.5f)));
    EXPECT_TRUE(sut.contains_point(sf::Vector2f(7.5f, 7.5f)));
}

TEST_F(CornersAndContainsPoint, Quadrant2)
{
    AABB sut({-8.f, 8.f}, dimensions_);

    EXPECT_EQ(sf::Vector2f(-9.f, 7.f), sut.get_min_corner());
    EXPECT_EQ(sf::Vector2f(-7.f, 9.f), sut.get_max_corner());

    EXPECT_EQ(sf::Vector2f(-7.f, 7.f), sut.get_near_corner());
    EXPECT_EQ(sf::Vector2f(-9.f, 9.f), sut.get_far_corner());

    EXPECT_TRUE(sut.contains_point(sf::Vector2f(-8.5f, 8.5f)));
    EXPECT_TRUE(sut.contains_point(sf::Vector2f(-7.5f, 8.5f)));
    EXPECT_TRUE(sut.contains_point(sf::Vector2f(-8.5f, 7.5f)));
    EXPECT_TRUE(sut.contains_point(sf::Vector2f(-7.5f, 7.5f)));
}

TEST_F(CornersAndContainsPoint, Quadrant3)
{
    AABB sut({-8.f, -8.f}, dimensions_);

    EXPECT_EQ(sf::Vector2f(-9.f, -9.f), sut.get_min_corner());
    EXPECT_EQ(sf::Vector2f(-7.f, -7.f), sut.get_max_corner());

    EXPECT_EQ(sf::Vector2f(-7.f, -7.f), sut.get_near_corner());
    EXPECT_EQ(sf::Vector2f(-9.f, -9.f), sut.get_far_corner());

    EXPECT_TRUE(sut.contains_point(sf::Vector2f(-8.5f, -8.5f)));
    EXPECT_TRUE(sut.contains_point(sf::Vector2f(-7.5f, -8.5f)));
    EXPECT_TRUE(sut.contains_point(sf::Vector2f(-8.5f, -7.5f)));
    EXPECT_TRUE(sut.contains_point(sf::Vector2f(-7.5f, -7.5f)));
}

TEST_F(CornersAndContainsPoint, Quadrant4)
{
    AABB sut({8.f, -8.f}, dimensions_);

    EXPECT_EQ(sf::Vector2f(7.f, -9.f), sut.get_min_corner());
    EXPECT_EQ(sf::Vector2f(9.f, -7.f), sut.get_max_corner());

    EXPECT_EQ(sf::Vector2f(7.f, -7.f), sut.get_near_corner());
    EXPECT_EQ(sf::Vector2f(9.f, -9.f), sut.get_far_corner());

    EXPECT_TRUE(sut.contains_point(sf::Vector2f(8.5f, -8.5f)));
    EXPECT_TRUE(sut.contains_point(sf::Vector2f(7.5f, -8.5f)));
    EXPECT_TRUE(sut.contains_point(sf::Vector2f(8.5f, -7.5f)));
    EXPECT_TRUE(sut.contains_point(sf::Vector2f(7.5f, -7.5f)));
}

TEST_F(CornersAndContainsPoint, Quadrant12)
{
    AABB sut({8.f, 0.f}, dimensions_);

    EXPECT_EQ(sf::Vector2f(7.f, -1.f), sut.get_min_corner());
    EXPECT_EQ(sf::Vector2f(9.f, 1.f), sut.get_max_corner());

    EXPECT_EQ(sf::Vector2f(7.f, -1.f), sut.get_near_corner());
    EXPECT_EQ(sf::Vector2f(9.f, 1.f), sut.get_far_corner());

    EXPECT_TRUE(sut.contains_point(sf::Vector2f(8.5f, 0.5f)));
    EXPECT_TRUE(sut.contains_point(sf::Vector2f(7.5f, 0.5f)));
    EXPECT_TRUE(sut.contains_point(sf::Vector2f(8.5f, -0.5f)));
    EXPECT_TRUE(sut.contains_point(sf::Vector2f(7.5f, -0.5f)));
}

TEST_F(CornersAndContainsPoint, Quadrant14)
{
    AABB sut({0.f, 8.f}, dimensions_);

    EXPECT_EQ(sf::Vector2f(-1.f, 7.f), sut.get_min_corner());
    EXPECT_EQ(sf::Vector2f(1.f, 9.f), sut.get_max_corner());

    EXPECT_EQ(sf::Vector2f(-1.f, 7.f), sut.get_near_corner());
    EXPECT_EQ(sf::Vector2f(1.f, 9.f), sut.get_far_corner());

    EXPECT_TRUE(sut.contains_point(sf::Vector2f(0.5f, 8.5f)));
    EXPECT_TRUE(sut.contains_point(sf::Vector2f(0.5f, 7.5f)));
    EXPECT_TRUE(sut.contains_point(sf::Vector2f(-0.5f, 8.5f)));
    EXPECT_TRUE(sut.contains_point(sf::Vector2f(-0.5f, 7.5f)));
}

class StateSpace : public TestableAABB
{
protected:
    static const sf::Vector2f position_;
    static const sf::Vector2f dimensions_;
    static const sf::Vector2f positive_trajectory_;
    static const sf::Vector2f negative_trajectory_;
    static const sf::Vector2f zero_trajectory_;
};
const sf::Vector2f StateSpace::position_            = {100.f, 200.f};
const sf::Vector2f StateSpace::dimensions_          = {10.f, 20.f};
const sf::Vector2f StateSpace::positive_trajectory_ = {1.f, 2.f};
const sf::Vector2f StateSpace::negative_trajectory_ = {-1.f, -2.f};
const sf::Vector2f StateSpace::zero_trajectory_     = {0.f, 0.f};

TEST_F(StateSpace, ZeroTrajectoryAABB_ReturnsACopyOfItself)
{
    AABB sut(position_, dimensions_, zero_trajectory_);
    AABB expected = sut;

    EXPECT_EQ(expected, AABB::state_space_for(sut));
}

TEST_F(StateSpace, NonZeroTrajectory_ReturnsTheMinimumStateSpace)
{
    AABB sut(position_, dimensions_, positive_trajectory_);
    AABB expected = AABB({100.5f, 201.f}, {11.f, 22.f});

    EXPECT_EQ(expected, AABB::state_space_for(sut));

    sut = AABB(position_, dimensions_, negative_trajectory_);
    expected = AABB({99.5f, 199.f}, {11.f, 22.f});

    EXPECT_EQ(expected, AABB::state_space_for(sut));
}

class MinkowskiDifference : public TestableAABB { };

TEST_F(MinkowskiDifference, OfItself_DoublesInSize_MovesToOrigin_HasNoTrajectory)
{
    sf::Vector2f position = {100.f, 200.f};
    sf::Vector2f dimensions = {10.f, 20.f};
    sf::Vector2f trajectory = {1.f, 2.f};

    AABB sut(position, dimensions, trajectory);
    AABB copy = sut;
    AABB expected({0.f, 0.f}, {20.f, 40.f});

    EXPECT_EQ(expected, AABB::minkowski_difference(sut, copy));
}

TEST_F(MinkowskiDifference, OfTwoDissimilarAABBs)
{
    sf::Vector2f position = {100.f, 200.f};
    sf::Vector2f dimensions = {10.f, 20.f};
    sf::Vector2f trajectory = {1.f, 2.f};
    AABB first(position, dimensions, trajectory);

    position = {-200.f, 400.f};
    dimensions = {20.f, 40.f};
    trajectory = {-1.f, 2.f};
    AABB second(position, dimensions, trajectory);

    AABB expected({300.f, -200.f}, {30.f, 60.f}, {2.f, 0.f});

    EXPECT_EQ(expected, AABB::minkowski_difference(first, second));
}
