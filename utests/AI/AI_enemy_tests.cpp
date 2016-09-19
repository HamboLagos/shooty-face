#include <gtest.h>
#include <gmock.h>

#include "AI/AI_enemy.hpp"

#include "enemy.hpp"
#include "mocks/collision_mock.hpp"
#include "mocks/entity_mock.hpp"
#include "game.hpp"
#include "components/physics.hpp"

using namespace testing;

class TestableAIEnemy : public Test
{
protected:
    Enemy enemy_;
    AIEnemy sut;

    TestableAIEnemy() :
        sut(enemy_)
    { }
};


class Update : public TestableAIEnemy
{
protected:
    static const sf::Vector2f initial_position_;
    static const sf::Vector2f target_;
    static const float speed_;

    Player* player_;

    Update() :
        player_(Game::instance().add_player())
    {
        auto* physics = enemy_.get_component<Physics>();
        physics->set_position(initial_position_);
        physics->set_move_speed(speed_);

        player_->get_component<Physics>()->set_position(target_);

        CollisionMock::set_defaults();
    }

    void TearDown() override
    {
        Game::instance().reset();
        CollisionMock::clear_defaults();
    }
};
const sf::Vector2f Update::initial_position_ = {10.f, 20.f};
const sf::Vector2f Update::target_           = {13.f, 24.f};
const float Update::speed_                   = 5.f;

TEST_F(Update, TracksThePlayerPosition)
{
    auto* physics = enemy_.get_component<Physics>();

    // each call to update moves it towards the player
    sut.update(0.5f);
    EXPECT_EQ(sf::Vector2f(11.5f, 22.f), physics->get_position());

    // at the player
    sut.update(0.5f);
    EXPECT_EQ(sf::Vector2f(13.f, 24.f), physics->get_position());

    // stays at the player
    sut.update(0.5f);
    EXPECT_EQ(sf::Vector2f(13.f, 24.f), physics->get_position());

    // player moves, enemy tracks the new position
    player_->get_component<Physics>()->set_position(initial_position_);
    sut.update(0.5f);
    EXPECT_EQ(sf::Vector2f(11.5f, 22.f), physics->get_position());

    sut.update(0.5f);
    EXPECT_EQ(sf::Vector2f(10.f, 20.f), physics->get_position());

    sut.update(0.5f);
    EXPECT_EQ(sf::Vector2f(10.f, 20.f), physics->get_position());
}

class Collisions : public TestableAIEnemy
{
protected:
    static const sf::Vector2f initial_position_;
    static const sf::Vector2f target_;
    static const float speed_;

    Player* player_;
    CollisionMock& collision_;
    Physics* physics;

    Collisions() :
        player_(Game::instance().add_player()),
        collision_(CollisionMock::instance()),
        physics(enemy_.get_component<Physics>())
    {
        physics->set_position(initial_position_);
        physics->set_move_speed(speed_);

        player_->get_component<Physics>()->set_position(target_);
    }

    void TearDown() override
    {
        Game::instance().reset();
        CollisionMock::clear_defaults();
    }
};
const sf::Vector2f Collisions::initial_position_ = {10.f, 20.f};
const sf::Vector2f Collisions::target_           = {13.f, 24.f};
const float Collisions::speed_                   = 5.f;

TEST_F(Collisions, WhenSanityCheckFails_DoesFullUpdate)
{
    EXPECT_CALL(collision_, sanity_check(Ref(enemy_), Ref(*player_))).WillOnce(Return(false));

    sut.update(1.f);
    EXPECT_EQ(sf::Vector2f(13.f, 24.f), physics->get_position());
}

TEST_F(Collisions, WhenBroadTestPasses_DoesFullUpdate)
{
    ON_CALL(collision_, sanity_check(Ref(enemy_), Ref(*player_))).WillByDefault(Return(true));

    EXPECT_CALL(collision_, broad_test(_, _)).WillOnce(Return(false));

    sut.update(1.f);
    EXPECT_EQ(sf::Vector2f(13.f, 24.f), physics->get_position());
}

TEST_F(Collisions, WhenNarrowTestPasses_DoesFullUpdate)
{
    ON_CALL(collision_, sanity_check(Ref(enemy_), Ref(*player_))).WillByDefault(Return(true));
    ON_CALL(collision_, broad_test(_, _)).WillByDefault(Return(true));

    EXPECT_CALL(collision_, narrow_test(_, _)).WillOnce(Return(1.f));

    sut.update(1.f);
    EXPECT_EQ(sf::Vector2f(13.f, 24.f), physics->get_position());
}

TEST_F(Collisions, WhenNarrowTestGivesGT0LT1_DoesPartialUpdate)
{
    ON_CALL(collision_, sanity_check(Ref(enemy_), Ref(*player_))).WillByDefault(Return(true));
    ON_CALL(collision_, broad_test(_, _)).WillByDefault(Return(true));

    EXPECT_CALL(collision_, narrow_test(_, _)).WillOnce(Return(0.5f));

    sut.update(1.f);
    EXPECT_EQ(sf::Vector2f(11.5f, 22.f), physics->get_position());
}

TEST_F(Collisions, WhenNarrowTestGives0_UsesResultsOfPenetrationTest)
{
    ON_CALL(collision_, sanity_check(Ref(enemy_), Ref(*player_))).WillByDefault(Return(true));
    ON_CALL(collision_, broad_test(_, _)).WillByDefault(Return(true));

    EXPECT_CALL(collision_, narrow_test(_, _)).WillOnce(Return(0.f));
    EXPECT_CALL(collision_, get_penetration(_, _)).WillOnce(Return(sf::Vector2f(1.f, 2.f)));

    sut.update(1.f);
    EXPECT_EQ(sf::Vector2f(9.f, 18.f), physics->get_position());
}

class MultiCollisions : public TestableAIEnemy
{
protected:
    static const sf::Vector2f initial_position_;
    static const sf::Vector2f target_;
    static const float speed_;

    Player* player_;
    EntityMock* entity_;
    CollisionMock& collision_;

    MultiCollisions() :
        player_(Game::instance().add_player()),
        entity_(new EntityMock),
        collision_(CollisionMock::instance())
    {
        auto* physics = enemy_.get_component<Physics>();
        physics->set_position(initial_position_);
        physics->set_move_speed(speed_);

        player_->get_component<Physics>()->set_position(target_);

        entity_->add_component<Physics>();
        Game::instance().entity_collection().push_back(std::unique_ptr<Entity>(entity_));
    }

    void TearDown() override
    {
        Game::instance().reset();
        CollisionMock::clear_defaults();
    }
};
const sf::Vector2f MultiCollisions::initial_position_ = {10.f, 20.f};
const sf::Vector2f MultiCollisions::target_           = {13.f, 24.f};
const float MultiCollisions::speed_                   = 5.f;

TEST_F(MultiCollisions, WhenBothFailNarrowTest_UsesTheSmallerTimeDelta1)
{
    ON_CALL(collision_, sanity_check(_, _)).WillByDefault(Return(true));
    ON_CALL(collision_, broad_test(_, _)).WillByDefault(Return(true));

    auto player_box = player_->get_component<Physics>()->get_box();
    auto entity_box = entity_->get_component<Physics>()->get_box();
    EXPECT_CALL(collision_, narrow_test(_, player_box)).WillOnce(Return(0.8f));
    EXPECT_CALL(collision_, narrow_test(_, entity_box)).WillOnce(Return(0.5f));

    sut.update(1.f);
    auto* physics = enemy_.get_component<Physics>();
    EXPECT_EQ(sf::Vector2f(11.5f, 22.f), physics->get_position());
}

TEST_F(MultiCollisions, WhenBothFailNarrowTest_UsesTheSmallerTimeDelta2)
{
    ON_CALL(collision_, sanity_check(_, _)).WillByDefault(Return(true));
    ON_CALL(collision_, broad_test(_, _)).WillByDefault(Return(true));

    auto player_box = player_->get_component<Physics>()->get_box();
    auto entity_box = entity_->get_component<Physics>()->get_box();
    EXPECT_CALL(collision_, narrow_test(_, player_box)).WillOnce(Return(0.8f));
    EXPECT_CALL(collision_, narrow_test(_, entity_box)).WillOnce(Return(0.5f));

    sut.update(2.f);
    auto* physics = enemy_.get_component<Physics>();
    EXPECT_EQ(sf::Vector2f(13.f, 24.f), physics->get_position());
}

TEST_F(MultiCollisions, WhenBothFailNarrowTest_AndOneReturns0_UsesThePentrationMovementOnly)
{
    ON_CALL(collision_, sanity_check(_, _)).WillByDefault(Return(true));
    ON_CALL(collision_, broad_test(_, _)).WillByDefault(Return(true));

    auto player_box = player_->get_component<Physics>()->get_box();
    auto entity_box = entity_->get_component<Physics>()->get_box();
    EXPECT_CALL(collision_, narrow_test(_, player_box)).WillOnce(Return(0.8f));
    EXPECT_CALL(collision_, narrow_test(_, entity_box)).WillOnce(Return(0.0f));
    EXPECT_CALL(collision_, get_penetration(_, entity_box)).WillOnce(Return(sf::Vector2f(1.f, 2.f)));

    sut.update(1.f);
    auto* physics = enemy_.get_component<Physics>();
    EXPECT_EQ(sf::Vector2f(9.f, 18.f), physics->get_position());
}
