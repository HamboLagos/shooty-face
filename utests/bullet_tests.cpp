#include <gtest.h>
#include <gmock.h>

#include "bullet.hpp"

using namespace testing;
using SUT = Bullet;

class TestableBullet : public Test
{
protected:
    auto& get_graphic(const Bullet& sut) { return sut.graphic_; }
};

class BeforeFiring : public TestableBullet {
protected:
    static const sf::Vector2f unused_v2f;
    static const float unused_float;
};
    const sf::Vector2f BeforeFiring::unused_v2f = {0.f, 0.f};
    const float BeforeFiring::unused_float      = 0.f;

TEST_F(BeforeFiring, InvisibleUntilFired)
{
    SUT sut(unused_v2f, unused_v2f, unused_float);

    sut.render();
    EXPECT_EQ(sf::Color::Transparent, get_graphic(sut).getFillColor());

    sut.fire();

    sut.render();
    EXPECT_NE(sf::Color::Transparent, get_graphic(sut).getFillColor());
}

class FiringAtTarget : public TestableBullet
{
protected:
    static const sf::Vector2f initial_position_;
    static const sf::Vector2f target_;
    static const float speed_;
};
const sf::Vector2f FiringAtTarget::initial_position_ = {10.f, 20.f};
const sf::Vector2f FiringAtTarget::target_           = {13.f, 24.f};
const float FiringAtTarget::speed_                   = 5.f;

TEST_F(FiringAtTarget, BulletProceedsInLinearTrajectoryThroughTarget)
{
    SUT sut(initial_position_, target_, speed_);
    EXPECT_TRUE(sut.is_dead());

    sut.fire();
    EXPECT_TRUE(sut.is_alive());
    EXPECT_EQ(sut.get_position(), sf::Vector2f(10.f, 20.f));
    EXPECT_EQ(sut.get_velocity(), sf::Vector2f(3.f, 4.f));

    // along trajectory to target
    sut.update(sf::seconds(0.5f));
    EXPECT_EQ(sut.get_position(), sf::Vector2f(11.5f, 22.f));

    // at target
    sut.update(sf::seconds(0.5f));
    EXPECT_EQ(sut.get_position(), sf::Vector2f(13.f, 24.f));

    // continues on trajectory through target
    sut.update(sf::seconds(0.5f));
    EXPECT_EQ(sut.get_position(), sf::Vector2f(14.5f, 26.f));
}
