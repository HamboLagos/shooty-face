#include <gtest.h>
#include <gmock.h>

#include "player.hpp"

using namespace testing;

class TestablePlayer : public Test
{
protected:
    Player sut;

    auto& get_graphic() { return sut.graphic_; }
};

class Transformations : public TestablePlayer
{
protected:
    static const sf::Vector2f position_;
    static const sf::Vector2f velocity_;
    static const sf::Time dt_;
};
const sf::Vector2f Transformations::position_ = {10.f, 20.f};
const sf::Vector2f Transformations::velocity_ = {1.f, 2.f};
const sf::Time Transformations::dt_ = sf::seconds(1.f);

TEST_F(Transformations, StartMoveBeginsAVelocityChange)
{
    sut.set_position(position_);
    sut.set_velocity(velocity_);

    sut.start_move(Player::Direction::DOWN);
    sut.start_move(Player::Direction::RIGHT);
    sut.update(dt_);

    EXPECT_EQ(sut.get_position(), sf::Vector2f({11.f, 22.f}));

    sut.stop_move(Player::Direction::DOWN);
    sut.stop_move(Player::Direction::RIGHT);
    sut.start_move(Player::Direction::UP);
    sut.start_move(Player::Direction::LEFT);
    sut.update(dt_);

    EXPECT_EQ(sut.get_position(), sf::Vector2f({10.f, 20.f}));
}

TEST_F(Transformations, StartMoveInOppositeDirectionsCancelEachOther)
{
    sut.set_position(position_);
    sut.set_velocity(velocity_);

    sut.start_move(Player::Direction::DOWN);
    sut.start_move(Player::Direction::RIGHT);
    sut.update(dt_);

    sut.start_move(Player::Direction::UP);
    sut.start_move(Player::Direction::LEFT);
    sut.update(dt_);

    EXPECT_EQ(sut.get_position(), sf::Vector2f({11.f, 22.f}));
}

TEST_F(Transformations, StopMoveCancelsMoveInGivenDirection)
{
    sut.set_position(position_);
    sut.set_velocity(velocity_);

    sut.start_move(Player::Direction::DOWN);
    sut.start_move(Player::Direction::RIGHT);
    sut.update(dt_);

    EXPECT_EQ(sut.get_position(), sf::Vector2f({11.f, 22.f}));

    sut.stop_move(Player::Direction::DOWN);
    sut.stop_move(Player::Direction::RIGHT);
    sut.update(dt_);

    EXPECT_EQ(sut.get_position(), sf::Vector2f({11.f, 22.f}));
}

class Render : public TestablePlayer
{
protected:
    static const sf::Vector2f position_;
    static const sf::Vector2f dimensions_;
};
const sf::Vector2f Render::position_ = {100.f, 200.f};
const sf::Vector2f Render::dimensions_ = {10.f, 20.f};

TEST_F(Render, InitializesGraphicWithCorrectPositionAndSize)
{
    sut.set_position(position_);
    sut.set_dimensions(dimensions_);
    auto& graphic = sut.render();

    EXPECT_EQ(get_graphic().getPosition(), sf::Vector2f(100.f, 200.f));
    EXPECT_EQ(get_graphic().getSize(), sf::Vector2f(10.f, 20.f));

    sut.move({10.f, 20.f});
    sut.scale(4.f);
    sut.render();

    EXPECT_EQ(get_graphic().getPosition(), sf::Vector2f(110.f, 220.f));
    EXPECT_EQ(get_graphic().getSize(), sf::Vector2f(40.f, 80.f));
}

class Shoot : public TestablePlayer
{
protected:
    static const sf::Vector2f position_;
    static const sf::Vector2f target_;
    static const float projectile_speed_;
};
const sf::Vector2f Shoot::position_  = {10.f, 20.f};
const sf::Vector2f Shoot::target_    = {13.f, 24.f};
const float Shoot::projectile_speed_ = 5.f;

TEST_F(Shoot, FiresAProjectileAtTheTarget)
{
    sut.set_position(position_);
    sut.set_projectile_speed(projectile_speed_);
    sut.shoot(target_);
    auto& projectile = *sut.get_projectile();

    EXPECT_EQ(projectile.get_position(), sf::Vector2f(10.f, 20.f));
    EXPECT_EQ(projectile.get_velocity(), sf::Vector2f(3.f, 4.f));

    projectile.update(sf::seconds(0.5f));
    EXPECT_EQ(projectile.get_position(), sf::Vector2f(11.5f, 22.f));
}
