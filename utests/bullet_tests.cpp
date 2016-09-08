#include <gtest.h>
#include <gmock.h>

#include "bullet.hpp"

using namespace testing;

class TestableBullet : public Test
{
protected:
    Bullet sut;
    auto& get_graphic(const Bullet& sut) { return sut.graphic_; }
};

class BeforeFiring : public TestableBullet { };

TEST_F(BeforeFiring, InvisibleUntilFired)
{
    sut.render();
    EXPECT_EQ(0, sut.get_renderings().size());

    sut.fire();
    sut.render();
    EXPECT_NE(0, sut.get_renderings().size());
}

class FiringAtTarget : public TestableBullet
{
protected:
    static const sf::Vector2f initial_position_;
    static const sf::Vector2f target_;
    static const float speed_;

    FiringAtTarget()
    {
        sut.set_position(initial_position_);
        sut.set_target(target_);
        sut.set_speed(speed_);
    }
};
const sf::Vector2f FiringAtTarget::initial_position_ = {10.f, 20.f};
const sf::Vector2f FiringAtTarget::target_           = {13.f, 24.f};
const float FiringAtTarget::speed_                   = 5.f;

TEST_F(FiringAtTarget, BulletProceedsInLinearTrajectoryThroughTarget)
{
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
