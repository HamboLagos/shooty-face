#include <gtest.h>
#include <gmock.h>

#include <player.hpp>

using namespace testing;

class TestablePlayer : public Test
{
public:
    Player sut;
};

class Transformations : public TestablePlayer { };

TEST_F(Transformations, SetPositionChangesAbsolutePosition)
{
    sut.set_position({100.f, 200.f});
    EXPECT_FLOAT_EQ(sut.get_position().x, 100.f);
    EXPECT_FLOAT_EQ(sut.get_position().y, 200.f);

    sut.set_position({900.f, 20.2f});
    EXPECT_FLOAT_EQ(sut.get_position().x, 900.f);
    EXPECT_FLOAT_EQ(sut.get_position().y, 20.2f);
}

TEST_F(Transformations, MoveChangesRelativePosition)
{
    sut.set_position({100.f, 200.f});
    sut.move({10.f, -20.f});
    EXPECT_FLOAT_EQ(sut.get_position().x, 110.f);
    EXPECT_FLOAT_EQ(sut.get_position().y, 180.f);
}

class Render : public TestablePlayer { };

TEST_F(Render, ReturnsAShapeWithCorrectPosition)
{
    sut.set_position({100.f, 200.f});
    auto graphic = sut.render();

    EXPECT_FLOAT_EQ(graphic.getPosition().x, 100.f);
    EXPECT_FLOAT_EQ(graphic.getPosition().y, 200.f);
}
