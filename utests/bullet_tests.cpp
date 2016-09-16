#include <gtest.h>
#include <gmock.h>

#include "bullet.hpp"
#include "components/phyics.hpp"

#include "mocks/clock_mock.hpp"

using namespace testing;

class TestableBullet : public Test
{
protected:
    Bullet sut;
};

class Components : public TestableBullet { };

TEST_F(Components, ExistenceThereof)
{
    EXPECT_TRUE(sut.has_component<Physics>());
    EXPECT_TRUE(sut.has_component<Graphics>());
}

TEST_F(Components, Physics_SetsDefaultMoveSpeed)
{
    auto* physics = sut.get_component<Physics>();
    EXPECT_FLOAT_EQ(Bullet::SPEED, physics->get_move_speed());
}

TEST_F(Components, Graphics_ReturnsACircleForThisBullet)
{
    auto* physics = sut.get_component<Physics>();

    physics->set_position({100.1f, 200.2f});
    physics->set_dimensions({10.1f, 20.2f});

    const auto renderings = sut.get_component<Graphics>()->render();
    const auto* graphic = static_cast<const sf::CircleShape*>(renderings[0]);

    EXPECT_EQ(sf::Vector2f(100.f, 200.f), graphic->getPosition());
    EXPECT_FLOAT_EQ(5.05f, graphic->getRadius());
}

class BeforeFiring : public TestableBullet { };

TEST_F(BeforeFiring, StaticUntilFired)
{
    EXPECT_TRUE(sut.get_component<Physics>()->is_static());

    sut.fire();

    EXPECT_TRUE(sut.get_component<Physics>()->is_dynamic());
}

class AfterFiring : public TestableBullet
{
protected:
    static const sf::Vector2f initial_position_;
    static const sf::Vector2f target_;
    static const float speed_;

    AfterFiring()
    {
        auto* physics = sut.get_component<Physics>();
        physics->set_position(initial_position_);
        physics->set_move_speed(speed_);

        sut.set_target(target_);
    }
};
const sf::Vector2f AfterFiring::initial_position_ = {10.f, 20.f};
const sf::Vector2f AfterFiring::target_           = {13.f, 24.f};
const float AfterFiring::speed_                   = 5.f;

TEST_F(AfterFiring, BulletProceedsInLinearTrajectoryThroughTarget)
{
    sut.fire();

    auto* physics = sut.get_component<Physics>();
    EXPECT_EQ(initial_position_, physics->get_position());
    EXPECT_EQ(sf::Vector2f(3.f, 4.f), physics->get_velocity());
}

class TestableBulletAmmunition : public Test
{
protected:
    NiceMock<ClockMock>* clock_;
    BulletAmmunition sut;

    std::vector<Projectile*> projectiles_;

    TestableBulletAmmunition() :
        clock_(new NiceMock<ClockMock>),
        sut(std::unique_ptr<ClockIF>(clock_))
    { }

    ~TestableBulletAmmunition()
    {
        for(auto* projectile : projectiles_) {
            delete projectile;
        }
    }
};

TEST_F(TestableBulletAmmunition, RestartsTheClockOnTheFirstCallAndAfterReload)
{
    EXPECT_CALL(*clock_, getElapsedTime()).Times(0);
    EXPECT_CALL(*clock_, restart()).Times(1);

    projectiles_.push_back(sut.create_projectile());
    EXPECT_NE(nullptr, projectiles_.back());

    sut.reload();

    EXPECT_CALL(*clock_, getElapsedTime()).Times(0);
    EXPECT_CALL(*clock_, restart()).Times(1);

    projectiles_.push_back(sut.create_projectile());
    EXPECT_NE(nullptr, projectiles_.back());
}

TEST_F(TestableBulletAmmunition, WontFireASecondTimeUntilTimeHasElapsed)
{
    projectiles_.push_back(sut.create_projectile());

    EXPECT_CALL(*clock_, getElapsedTime()).WillOnce(Return(sf::Time::Zero));
    EXPECT_CALL(*clock_, restart()).Times(0);

    projectiles_.push_back(sut.create_projectile());
    EXPECT_EQ(nullptr, projectiles_.back());

    EXPECT_CALL(*clock_, getElapsedTime())
        .WillOnce(Return(sf::seconds(1.f/(2.f * BulletAmmunition::FIRE_RATE))));
    EXPECT_CALL(*clock_, restart()).Times(0);

    projectiles_.push_back(sut.create_projectile());
    EXPECT_EQ(nullptr, projectiles_.back());

    EXPECT_CALL(*clock_, getElapsedTime())
        .WillOnce(Return(sf::seconds(1.f/BulletAmmunition::FIRE_RATE)));
    EXPECT_CALL(*clock_, restart()).Times(1);

    projectiles_.push_back(sut.create_projectile());
    EXPECT_NE(nullptr, projectiles_.back());
}
