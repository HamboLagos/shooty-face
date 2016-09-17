#include <gtest.h>
#include <gmock.h>

#include "bullet.hpp"

#include "components/health.hpp"
#include "components/physics.hpp"
#include "game.hpp"
#include "mocks/clock_mock.hpp"
#include "mocks/collision_mock.hpp"
#include "mocks/entity_mock.hpp"

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

    // each call to update moves it along the trajectory
    sut.update(sf::seconds(0.5f));
    EXPECT_EQ(sf::Vector2f(11.5f, 22.f), physics->get_position());

    // at the target point
    sut.update(sf::seconds(0.5f));
    EXPECT_EQ(sf::Vector2f(13.f, 24.f), physics->get_position());

    // travels through the target point
    sut.update(sf::seconds(1.f));
    EXPECT_EQ(sf::Vector2f(16.f, 28.f), physics->get_position());

    // until it dies
    sut.kill();
    sut.update(sf::seconds(1.f));
    EXPECT_EQ(sf::Vector2f(16.f, 28.f), physics->get_position());
}

class Collisions : public TestableBullet
{
protected:
    static const sf::Vector2f initial_position_;
    static const sf::Vector2f target_;
    static const float speed_;

    CollisionMock& collision_;
    EntityMock* entity_;

    Collisions() :
        collision_(CollisionMock::instance()),
        entity_(new EntityMock)
    {
        auto* physics = sut.get_component<Physics>();
        physics->set_position(initial_position_);
        physics->set_move_speed(speed_);

        sut.set_target(target_);

        entity_->add_component<Physics>();
        Game::instance().entity_collection().push_back(std::unique_ptr<Entity>(entity_));
    }

    ~Collisions()
    {
        Game::instance().entity_collection().clear();
    }
};
const sf::Vector2f Collisions::initial_position_  = {10.f, 10.f};
const sf::Vector2f Collisions::target_            = {15.f, 10.f};
const float Collisions::speed_                    = 4.f;

TEST_F(Collisions, NoCollisionUntilNarrowTestFails)
{
    sut.fire();

    EXPECT_CALL(collision_, sanity_check(Ref(sut), Ref(*entity_))).WillOnce(Return(false));

    sut.update(sf::Time::Zero);
    EXPECT_TRUE(sut.is_alive());

    auto box = sut.get_component<Physics>()->get_box(1.f);
    EXPECT_CALL(collision_, sanity_check(Ref(sut), Ref(*entity_))).WillOnce(Return(true));
    EXPECT_CALL(collision_, broad_test(box, _)).WillOnce(Return(false));

    sut.update(sf::seconds(1.f));
    EXPECT_TRUE(sut.is_alive());
    EXPECT_EQ(sf::Vector2f(14.f, 10.f), sut.get_component<Physics>()->get_position());

    box = sut.get_component<Physics>()->get_box(1.f);
    EXPECT_CALL(collision_, sanity_check(Ref(sut), Ref(*entity_))).WillOnce(Return(true));
    EXPECT_CALL(collision_, broad_test(box, _)).WillOnce(Return(true));
    EXPECT_CALL(collision_, narrow_test(box, _)).WillOnce(Return(1.f));

    sut.update(sf::seconds(1.f));
    EXPECT_TRUE(sut.is_alive());
    EXPECT_EQ(sf::Vector2f(18.f, 10.f), sut.get_component<Physics>()->get_position());

    box = sut.get_component<Physics>()->get_box(1.f);
    EXPECT_CALL(collision_, sanity_check(Ref(sut), Ref(*entity_))).WillOnce(Return(true));
    EXPECT_CALL(collision_, broad_test(box, _)).WillOnce(Return(true));
    EXPECT_CALL(collision_, narrow_test(box, _)).WillOnce(Return(0.5f));

    sut.update(sf::seconds(1.f));
    EXPECT_TRUE(sut.is_dead());
    EXPECT_EQ(sf::Vector2f(20.f, 10.f), sut.get_component<Physics>()->get_position());
}

TEST_F(Collisions, DealsDamageOnCollision)
{
    entity_->add_component<Health>();
    sut.fire();

    EXPECT_CALL(collision_, sanity_check(Ref(sut), Ref(*entity_))).WillOnce(Return(true));
    EXPECT_CALL(collision_, broad_test(_, _)).WillOnce(Return(true));
    EXPECT_CALL(collision_, narrow_test(_, _)).WillOnce(Return(0.f));

    sut.update(sf::Time::Zero);

    float health = 100.f - Bullet::DAMAGE;
    EXPECT_EQ(health, entity_->get_component<Health>()->get_health());
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
    // first call always succeeds
    EXPECT_CALL(*clock_, getElapsedTime()).Times(0);
    EXPECT_CALL(*clock_, restart()).Times(1);

    projectiles_.push_back(sut.create_projectile());
    EXPECT_NE(nullptr, projectiles_.back());

    // suceed on first call after a reload too
    sut.reload();

    EXPECT_CALL(*clock_, getElapsedTime()).Times(0);
    EXPECT_CALL(*clock_, restart()).Times(1);

    projectiles_.push_back(sut.create_projectile());
    EXPECT_NE(nullptr, projectiles_.back());
}

TEST_F(TestableBulletAmmunition, WontFireASecondTimeUntilTimeHasElapsed)
{
    // first call always succeeds
    projectiles_.push_back(sut.create_projectile());

    // fails when no time has passed
    EXPECT_CALL(*clock_, getElapsedTime()).WillOnce(Return(sf::Time::Zero));
    EXPECT_CALL(*clock_, restart()).Times(0);

    projectiles_.push_back(sut.create_projectile());
    EXPECT_EQ(nullptr, projectiles_.back());

    // fails when not enough time has passed
    EXPECT_CALL(*clock_, getElapsedTime())
        .WillOnce(Return(sf::seconds(1.f/(2.f * BulletAmmunition::FIRE_RATE))));
    EXPECT_CALL(*clock_, restart()).Times(0);

    projectiles_.push_back(sut.create_projectile());
    EXPECT_EQ(nullptr, projectiles_.back());

    // succeeds when enough time has passed
    EXPECT_CALL(*clock_, getElapsedTime())
        .WillOnce(Return(sf::seconds(1.f/BulletAmmunition::FIRE_RATE)));
    EXPECT_CALL(*clock_, restart()).Times(1);

    projectiles_.push_back(sut.create_projectile());
    EXPECT_NE(nullptr, projectiles_.back());
}
