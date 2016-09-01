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

TEST_F(Transformations, StartMoveBeginsAVelocityChange)
{
    // start at <0, 0>
    sut.set_position({0.f, 0.f});
    sut.set_velocity({1.f, 1.f});

    sut.start_move(Player::Direction::UP);
    sut.start_move(Player::Direction::RIGHT);
    sut.update();

    EXPECT_FLOAT_EQ(sut.get_position().x, 1.f);
    EXPECT_FLOAT_EQ(sut.get_position().y, -1.f);

    sut.update();
    sut.update();

    EXPECT_FLOAT_EQ(sut.get_position().x, 3.f);
    EXPECT_FLOAT_EQ(sut.get_position().y, -3.f);

    // active move in opposite directions cancels movement
    sut.start_move(Player::Direction::DOWN);
    sut.start_move(Player::Direction::LEFT);

    sut.update();

    EXPECT_FLOAT_EQ(sut.get_position().x, 3.f);
    EXPECT_FLOAT_EQ(sut.get_position().y, -3.f);

    // stop original movements, secondary movements active
    sut.stop_move(Player::Direction::UP);
    sut.stop_move(Player::Direction::RIGHT);
    sut.update();

    EXPECT_FLOAT_EQ(sut.get_position().x, 2.f);
    EXPECT_FLOAT_EQ(sut.get_position().y, -2.f);

    // stop secondary movements, now stationary
    sut.stop_move(Player::Direction::DOWN);
    sut.stop_move(Player::Direction::LEFT);
    sut.update();

    EXPECT_FLOAT_EQ(sut.get_position().x, 2.f);
    EXPECT_FLOAT_EQ(sut.get_position().y, -2.f);
}

TEST_F(Transformations, SetVelocityChangesTheMoveSpeed)
{
    sut.set_position({0.f, 0.f});
    sut.set_velocity({0.f, 0.f});
    sut.start_move(Player::Direction::RIGHT);
    sut.start_move(Player::Direction::DOWN);
    sut.update();

    EXPECT_FLOAT_EQ(sut.get_position().x, 0.f);
    EXPECT_FLOAT_EQ(sut.get_position().y, 0.f);

    sut.set_velocity({1.f, 2.f});
    sut.update();

    EXPECT_FLOAT_EQ(sut.get_position().x, 1.f);
    EXPECT_FLOAT_EQ(sut.get_position().y, 2.f);
}

class Render : public TestablePlayer { };

TEST_F(Render, ReturnsAShapeWithCorrectPosition)
{
    sut.set_position({100.f, 200.f});
    auto& graphic = sut.render();

    EXPECT_FLOAT_EQ(graphic.getPosition().x, 100.f);
    EXPECT_FLOAT_EQ(graphic.getPosition().y, 200.f);
}
