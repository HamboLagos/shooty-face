#include <gtest.h>
#include <gmock.h>

#include "AI/AI_bullet.hpp"

#include "bullet.hpp"
#include "components/health.hpp"
#include "components/physics.hpp"
#include "game.hpp"
#include "mocks/collision_mock.hpp"
#include "mocks/entity_mock.hpp"

using namespace testing;

class TestableAIBullet : public Test
{
protected:
    Bullet bullet_;
    AIBullet sut;

    TestableAIBullet() :
        sut(bullet_)
    { }
};

class Update : public TestableAIBullet
{
protected:
    static const sf::Vector2f initial_position_;
    static const sf::Vector2f target_;
    static const float speed_;

    Update()
    {
        auto* physics = bullet_.get_component<Physics>();
        physics->set_position(initial_position_);
        physics->set_move_speed(speed_);

        bullet_.set_target(target_);
        bullet_.fire();
    }
};
const sf::Vector2f Update::initial_position_ = {10.f, 20.f};
const sf::Vector2f Update::target_           = {13.f, 24.f};
const float Update::speed_                   = 5.f;

TEST_F(Update, BulletProceedsInLinearTrajectoryThroughTarget)
{
    auto* physics = bullet_.get_component<Physics>();
    EXPECT_EQ(initial_position_, physics->get_position());
    EXPECT_EQ(sf::Vector2f(3.f, 4.f), physics->get_velocity());

    // each call to update moves it along the trajectory
    sut.update(0.5f);
    EXPECT_EQ(sf::Vector2f(11.5f, 22.f), physics->get_position());

    // at the target point
    sut.update(0.5f);
    EXPECT_EQ(sf::Vector2f(13.f, 24.f), physics->get_position());

    // travels through the target point
    sut.update(1.f);
    EXPECT_EQ(sf::Vector2f(16.f, 28.f), physics->get_position());

    // until it dies
    bullet_.kill();
    sut.update(1.f);
    EXPECT_EQ(sf::Vector2f(16.f, 28.f), physics->get_position());
}

class Collisions : public TestableAIBullet
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
        auto* physics = bullet_.get_component<Physics>();
        physics->set_position(initial_position_);
        physics->set_move_speed(speed_);

        bullet_.set_target(target_);
        bullet_.fire();

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
    EXPECT_CALL(collision_, sanity_check(Ref(bullet_), Ref(*entity_))).WillOnce(Return(false));

    sut.update(0.f);
    EXPECT_TRUE(bullet_.is_alive());

    auto box = bullet_.get_component<Physics>()->get_box(1.f);
    EXPECT_CALL(collision_, sanity_check(Ref(bullet_), Ref(*entity_))).WillOnce(Return(true));
    EXPECT_CALL(collision_, broad_test(box, _)).WillOnce(Return(false));

    sut.update(1.f);
    EXPECT_TRUE(bullet_.is_alive());
    EXPECT_EQ(sf::Vector2f(14.f, 10.f), bullet_.get_component<Physics>()->get_position());

    box = bullet_.get_component<Physics>()->get_box(1.f);
    EXPECT_CALL(collision_, sanity_check(Ref(bullet_), Ref(*entity_))).WillOnce(Return(true));
    EXPECT_CALL(collision_, broad_test(box, _)).WillOnce(Return(true));
    EXPECT_CALL(collision_, narrow_test(box, _)).WillOnce(Return(1.f));

    sut.update(1.f);
    EXPECT_TRUE(bullet_.is_alive());
    EXPECT_EQ(sf::Vector2f(18.f, 10.f), bullet_.get_component<Physics>()->get_position());

    box = bullet_.get_component<Physics>()->get_box(1.f);
    EXPECT_CALL(collision_, sanity_check(Ref(bullet_), Ref(*entity_))).WillOnce(Return(true));
    EXPECT_CALL(collision_, broad_test(box, _)).WillOnce(Return(true));
    EXPECT_CALL(collision_, narrow_test(box, _)).WillOnce(Return(0.5f));

    sut.update(1.f);
    EXPECT_TRUE(bullet_.is_dead());
    EXPECT_EQ(sf::Vector2f(20.f, 10.f), bullet_.get_component<Physics>()->get_position());
}

TEST_F(Collisions, DealsDamageOnCollision)
{
    entity_->add_component<Health>()->set_health(50.f);

    EXPECT_CALL(collision_, sanity_check(Ref(bullet_), Ref(*entity_))).WillOnce(Return(true));
    EXPECT_CALL(collision_, broad_test(_, _)).WillOnce(Return(true));
    EXPECT_CALL(collision_, narrow_test(_, _)).WillOnce(Return(0.f));

    sut.update(0.f);

    EXPECT_EQ(50.f-Bullet::DAMAGE, entity_->get_component<Health>()->get_health());
}

