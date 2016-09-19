#include <gtest.h>
#include <gmock.h>

#include "bullet.hpp"

#include "components/AI.hpp"
#include "components/health.hpp"
#include "components/physics.hpp"
#include "game.hpp"
#include "mocks/clock_mock.hpp"
#include "mocks/collision_mock.hpp"
#include "mocks/entity_mock.hpp"
#include "mocks/ai_mock.hpp"

using namespace testing;

class TestableBullet : public Test
{
protected:
    Bullet sut;
};

class Components : public TestableBullet { };

TEST_F(Components, ExistenceThereof)
{
    EXPECT_TRUE(sut.has_component<AI>());
    EXPECT_TRUE(sut.has_component<Physics>());
    EXPECT_TRUE(sut.has_component<Graphics>());
}

TEST_F(Components, Physics_SetsDefaults)
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

class Fire : public TestableBullet { };

TEST_F(Fire, EnablesMovementForBulletByAI)
{
    EXPECT_TRUE(sut.get_component<Physics>()->is_static());
    sut.fire();
    EXPECT_TRUE(sut.get_component<Physics>()->is_dynamic());
}

class Update : public TestableBullet
{
protected:
    AIMock* ai_;

    Update() :
        ai_(new AIMock(sut))
    {
        sut.set_component<AI>(std::unique_ptr<AI>(ai_));
    }
};

TEST_F(Update, DelegatesToAI)
{
    EXPECT_CALL(*ai_, update(0.5f));
    sut.update(sf::seconds(0.5f));
}

class ApplyDamage : public TestableBullet { };

TEST_F(ApplyDamage, DamagesTheGivenEntityIfItHasHealth)
{
    EntityMock entity;
    sut.apply_damage(entity);

    entity.add_component<Health>()->set_health(50.f);
    sut.apply_damage(entity);
    EXPECT_EQ(50.f - Bullet::DAMAGE, entity.get_component<Health>()->get_health());
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
