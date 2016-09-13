#include <gtest.h>
#include <gmock.h>

#include "entity.hpp"

class StubEntity : public Entity {
    void update(sf::Time elapsed) override { return; }
};

using namespace testing;

class TestableEntity : public Test
{
protected:
    StubEntity sut;
};

class Transformations : public TestableEntity
{
protected:
    static const sf::Vector2f delta_;
    static const sf::Vector2f nominal_;
};

const sf::Vector2f Transformations::delta_ = {10.f, 20.f};
const sf::Vector2f Transformations::nominal_ = {100.f, 200.f};

TEST_F(Transformations, ChangingDimensions)
{
    sut.set_dimensions(nominal_);
    EXPECT_EQ(sut.get_dimensions(), sf::Vector2f(100.f, 200.f));
    EXPECT_EQ(sut.get_extents(), sf::Vector2f(50.f, 100.f));

    sut.set_dimensions(nominal_);
    sut.enlarge(delta_);
    EXPECT_EQ(sut.get_dimensions(), sf::Vector2f(110.f, 220.f));

    sut.set_dimensions(nominal_);
    sut.scale(delta_.x);
    EXPECT_EQ(sut.get_dimensions(), sf::Vector2f(1000.f, 2000.f));

    sut.set_dimensions(nominal_);
    sut.scale(delta_);
    EXPECT_EQ(sut.get_dimensions(), sf::Vector2f(1000.f, 4000.f));
}

TEST_F(Transformations, ChangingPosition)
{
    sut.set_position(nominal_);
    EXPECT_EQ(sut.get_position(), sf::Vector2f(100.f, 200.f));

    sut.set_position(nominal_);
    sut.move(delta_);
    EXPECT_EQ(sut.get_position(), sf::Vector2f(110.f, 220.f));
}

TEST_F(Transformations, ChangingVelocity)
{
    sut.set_velocity(nominal_);
    EXPECT_EQ(sut.get_velocity(), sf::Vector2f(100.f, 200.f));

    sut.set_velocity(nominal_);
    sut.speed_up(delta_);
    EXPECT_EQ(sut.get_velocity(), sf::Vector2f(110.f, 220.f));
}

class BoundingBox : public TestableEntity
{
protected:
    static const sf::Vector2f position_;
    static const sf::Vector2f dimensions_;
    static const sf::Vector2f velocity_;
};
const sf::Vector2f BoundingBox::position_ = {100.f, 200.f};
const sf::Vector2f BoundingBox::dimensions_ = {10.f, 20.f};
const sf::Vector2f BoundingBox::velocity_ = {1.f, 2.f};

TEST_F(BoundingBox, IsCalculatedFromTheEntityProperties)
{
    sut.set_position(position_);
    sut.set_dimensions(dimensions_);
    sut.set_velocity(velocity_);
    EXPECT_EQ(sut.get_box(sf::seconds(0.5f)), AABB({100.f, 200.f}, {10.f, 20.f}, {0.5f, 1.f}));

    sut.move({10.f, 20.f});
    sut.scale(4.f);
    EXPECT_EQ(sut.get_box(sf::seconds(0.25f)), AABB({110.f, 220.f}, {40.f, 80.f}, {0.25f, 0.5f}));
}

class Aliveness : public TestableEntity { };

TEST_F(Aliveness, AnimatingAndKiling)
{
    sut.kill();
    EXPECT_FALSE(sut.is_alive());
    EXPECT_TRUE(sut.is_dead());

    sut.set_alive(true);
    EXPECT_TRUE(sut.is_alive());
    EXPECT_FALSE(sut.is_dead());

    sut.set_alive(false);
    EXPECT_FALSE(sut.is_alive());
    EXPECT_TRUE(sut.is_dead());
}
