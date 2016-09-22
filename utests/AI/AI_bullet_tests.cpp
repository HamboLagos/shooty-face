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
    sut.update(sf::seconds(0.5f));
    EXPECT_EQ(sf::Vector2f(11.5f, 22.f), physics->get_position());

    // at the target point
    sut.update(sf::seconds(0.5f));
    EXPECT_EQ(sf::Vector2f(13.f, 24.f), physics->get_position());

    // travels through the target point
    sut.update(sf::seconds(1.f));
    EXPECT_EQ(sf::Vector2f(16.f, 28.f), physics->get_position());

    // until it dies
    bullet_.kill();
    sut.update(sf::seconds(1.f));
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

    virtual ~Collisions()
    {
        Game::instance().reset();
        CollisionMock::clear_defaults();
    }
};
const sf::Vector2f Collisions::initial_position_  = {10.f, 10.f};
const sf::Vector2f Collisions::target_            = {15.f, 10.f};
const float Collisions::speed_                    = 4.f;

TEST_F(Collisions, WhenSanityCheckFails_DoesFullUpdate)
{
    EXPECT_CALL(collision_, sanity_check(Ref(bullet_), Ref(*entity_))).WillOnce(Return(false));

    sut.update(sf::seconds(2.f));
    EXPECT_TRUE(bullet_.is_alive());
    EXPECT_EQ(sf::Vector2f(18.f, 10.f), bullet_.get_component<Physics>()->get_position());
}

TEST_F(Collisions, WhenBroadTestPasses_DoesFullUpdate)
{
    ON_CALL(collision_, sanity_check(Ref(bullet_), Ref(*entity_))).WillByDefault(Return(true));

    auto box = bullet_.get_component<Physics>()->get_box(2.f);
    EXPECT_CALL(collision_, broad_test(box, _)).WillOnce(Return(false));

    sut.update(sf::seconds(2.f));
    EXPECT_TRUE(bullet_.is_alive());
    EXPECT_EQ(sf::Vector2f(18.f, 10.f), bullet_.get_component<Physics>()->get_position());
}

TEST_F(Collisions, WhenNarrowTestPasses_DoesFullUpdate)
{
    auto box = bullet_.get_component<Physics>()->get_box(2.f);

    ON_CALL(collision_, sanity_check(Ref(bullet_), Ref(*entity_))).WillByDefault(Return(true));
    ON_CALL(collision_, broad_test(box, _)).WillByDefault(Return(true));

    EXPECT_CALL(collision_, narrow_test(box, _)).WillOnce(Return(1.f));

    sut.update(sf::seconds(2.f));
    EXPECT_TRUE(bullet_.is_alive());
    EXPECT_EQ(sf::Vector2f(18.f, 10.f), bullet_.get_component<Physics>()->get_position());
}

TEST_F(Collisions, WhenNarrowTestGivesGT0LT1_DoesPartialUpdate_ThenDamagesEntity)
{
    entity_->add_component<Health>()->set_health(50.f);

    auto box = bullet_.get_component<Physics>()->get_box(2.f);

    ON_CALL(collision_, sanity_check(Ref(bullet_), Ref(*entity_))).WillByDefault(Return(true));
    ON_CALL(collision_, broad_test(box, _)).WillByDefault(Return(true));

    EXPECT_CALL(collision_, narrow_test(box, _)).WillOnce(Return(0.5f));

    sut.update(sf::seconds(2.f));
    EXPECT_TRUE(bullet_.is_dead());
    EXPECT_EQ(sf::Vector2f(14.f, 10.f), bullet_.get_component<Physics>()->get_position());

    EXPECT_EQ(50.f-Bullet::DAMAGE, entity_->get_component<Health>()->get_health());
}

TEST_F(Collisions, WhenNarrowTestGives0_DamagesTheEntity)
{
    entity_->add_component<Health>()->set_health(50.f);

    auto box = bullet_.get_component<Physics>()->get_box(2.f);

    ON_CALL(collision_, sanity_check(Ref(bullet_), Ref(*entity_))).WillByDefault(Return(true));
    ON_CALL(collision_, broad_test(box, _)).WillByDefault(Return(true));

    EXPECT_CALL(collision_, narrow_test(box, _)).WillOnce(Return(0.f));

    sut.update(sf::seconds(2.f));
    EXPECT_TRUE(bullet_.is_dead());
    EXPECT_EQ(sf::Vector2f(10.f, 10.f), bullet_.get_component<Physics>()->get_position());

    EXPECT_EQ(50.f-Bullet::DAMAGE, entity_->get_component<Health>()->get_health());
}

class MultiCollisions : public TestableAIBullet
{
protected:
    static const sf::Vector2f initial_position_;
    static const sf::Vector2f target_;
    static const float speed_;

    CollisionMock& collision_;
    EntityMock* entity1_;
    EntityMock* entity2_;
    Player* player_;

    MultiCollisions() :
        collision_(CollisionMock::instance()),
        entity1_(new EntityMock),
        entity2_(new EntityMock),
        player_(Game::instance().add_player())
    {
        auto* physics = bullet_.get_component<Physics>();
        physics->set_position(initial_position_);
        physics->set_move_speed(speed_);

        bullet_.set_target(target_);
        bullet_.fire();

        entity1_->add_component<Physics>()->set_position({0.f, 0.f});
        entity1_->set_component<Health>(std::make_unique<Health>(*entity1_, 50.f));
        Game::instance().entity_collection().push_back(std::unique_ptr<Entity>(entity1_));

        entity2_->add_component<Physics>()->set_position({10.f, 10.f});
        entity2_->set_component<Health>(std::make_unique<Health>(*entity2_, 50.f));
        Game::instance().entity_collection().push_back(std::unique_ptr<Entity>(entity2_));
    }

    virtual ~MultiCollisions()
    {
        Game::instance().reset();
        CollisionMock::clear_defaults();
    }
};
const sf::Vector2f MultiCollisions::initial_position_  = {10.f, 10.f};
const sf::Vector2f MultiCollisions::target_            = {15.f, 10.f};
const float MultiCollisions::speed_                    = 4.f;

TEST_F(MultiCollisions, SkipsCollisionsWithThePlayer)
{
    EXPECT_EQ(3, Game::instance().entities().size());
    EXPECT_CALL(collision_, sanity_check(Ref(bullet_), Ref(*entity1_)));
    EXPECT_CALL(collision_, sanity_check(Ref(bullet_), Ref(*entity2_)));
    // player gets skipped:
    // EXPECT_CALL(collision_, sanity_check(Ref(bullet_), Ref(player_)));

    sut.update(sf::seconds(2.f));
}

TEST_F(MultiCollisions, OnlyDamagesTheFirstEntityItHits)
{
    ON_CALL(collision_, sanity_check(Ref(bullet_), _)).WillByDefault(Return(true));
    ON_CALL(collision_, broad_test(_, _)).WillByDefault(Return(true));

    auto box1 = entity1_->get_component<Physics>()->get_box();
    auto box2 = entity2_->get_component<Physics>()->get_box();
    EXPECT_CALL(collision_, narrow_test(_, box1)).WillOnce(Return(0.8f));
    EXPECT_CALL(collision_, narrow_test(_, box2)).WillOnce(Return(0.5f));

    sut.update(sf::seconds(2.f));

    EXPECT_EQ(50.f, entity1_->get_component<Health>()->get_health());
    EXPECT_EQ(50.f-Bullet::DAMAGE, entity2_->get_component<Health>()->get_health());
}
