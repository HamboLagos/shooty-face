#include <gtest.h>
#include <gmock.h>

#include "player.hpp"

#include "components/physics.hpp"
#include "game.hpp"
#include "mocks/collision_mock.hpp"
#include "mocks/entity_mock.hpp"
#include "utils.hpp"

using namespace testing;

class TestablePlayer : public Test
{
protected:
    Player sut;

    TestablePlayer()
    {
        CollisionMock::set_defaults();
    }

    virtual ~TestablePlayer()
    {
        Game::instance().reset();
        CollisionMock::clear_defaults();
    }
};

class Components : public TestablePlayer { };

TEST_F(Components, ExistenceThereof)
{
    EXPECT_TRUE(sut.has_component<Physics>());
    EXPECT_TRUE(sut.has_component<Graphics>());
}

TEST_F(Components, Physics_SetsDefaults)
{
    auto* physics = sut.get_component<Physics>();
    EXPECT_FLOAT_EQ(Player::SPEED, physics->get_move_speed());
}

TEST_F(Components, Graphics_ReturnsRectangleForThisPlayer)
{
    auto* physics = sut.get_component<Physics>();

    physics->set_position({100.1f, 200.2f});
    physics->set_dimensions({10.1f, 20.2f});

    const auto renderings = sut.get_component<Graphics>()->render();
    const auto* graphic = static_cast<const sf::RectangleShape*>(renderings[0]);

    EXPECT_EQ(sf::Vector2f(100.f, 200.f), graphic->getPosition());
    EXPECT_EQ(sf::Vector2f(10.1f, 20.2f), graphic->getSize());
}

class Movement : public TestablePlayer
{
protected:
    static const sf::Vector2f initial_position_;
    static const float move_speed_;

    Movement()
    {
        auto* physics = sut.get_component<Physics>();
        physics->set_position(initial_position_);
        physics->set_move_speed(move_speed_);
    }
};
const sf::Vector2f Movement::initial_position_ = {10.f, 20.f};
const float Movement::move_speed_               = 10.f;

TEST_F(Movement, StartMoveBeginsAVelocityChange)
{
    sut.start_move(Player::Direction::DOWN);
    sut.update(sf::seconds(0.5f));
    sut.stop_move(Player::Direction::DOWN);

    EXPECT_EQ(sf::Vector2f(10.f, 25.f), sut.get_component<Physics>()->get_position());

    sut.start_move(Player::Direction::UP);
    sut.update(sf::seconds(0.5f));
    sut.stop_move(Player::Direction::UP);

    EXPECT_EQ(sf::Vector2f(10.f, 20.f), sut.get_component<Physics>()->get_position());

    sut.start_move(Player::Direction::RIGHT);
    sut.update(sf::seconds(0.5f));
    sut.stop_move(Player::Direction::RIGHT);

    EXPECT_EQ(sf::Vector2f(15.f, 20.f), sut.get_component<Physics>()->get_position());

    sut.start_move(Player::Direction::LEFT);
    sut.update(sf::seconds(0.5f));
    sut.stop_move(Player::Direction::LEFT);

    EXPECT_EQ(sf::Vector2f(10.f, 20.f), sut.get_component<Physics>()->get_position());

    // along the diagonals
    auto distance = util::direction(sf::Vector2f(1.f, 1.f)) * move_speed_ * 0.5f;
    sut.start_move(Player::Direction::DOWN);
    sut.start_move(Player::Direction::RIGHT);
    sut.update(sf::seconds(0.5f));
    sut.stop_move(Player::Direction::DOWN);
    sut.stop_move(Player::Direction::RIGHT);

    EXPECT_EQ(sf::Vector2f(10.f, 20.f) + distance, sut.get_component<Physics>()->get_position());

    sut.start_move(Player::Direction::UP);
    sut.start_move(Player::Direction::LEFT);
    sut.update(sf::seconds(0.5f));
    sut.stop_move(Player::Direction::UP);
    sut.stop_move(Player::Direction::LEFT);

    EXPECT_EQ(sf::Vector2f(10.f, 20.f), sut.get_component<Physics>()->get_position());
}

TEST_F(Movement, StartMoveInOppositeDirectionsCancelEachOther)
{
    sut.start_move(Player::Direction::LEFT);
    sut.start_move(Player::Direction::RIGHT);
    sut.update(sf::seconds(0.5f));
    sut.stop_move(Player::Direction::LEFT);
    sut.stop_move(Player::Direction::RIGHT);

    EXPECT_EQ(sf::Vector2f(10.f, 20.f), sut.get_component<Physics>()->get_position());

    sut.start_move(Player::Direction::UP);
    sut.start_move(Player::Direction::DOWN);
    sut.update(sf::seconds(0.5f));
    sut.stop_move(Player::Direction::UP);
    sut.stop_move(Player::Direction::DOWN);

    EXPECT_EQ(sf::Vector2f(10.f, 20.f), sut.get_component<Physics>()->get_position());
}

class Collisions : public TestablePlayer
{
protected:
    static const sf::Vector2f initial_position_;
    static const float speed_;

    CollisionMock& collision_;
    EntityMock* entity_;

    Collisions() :
        collision_(CollisionMock::instance()),
        entity_(new EntityMock())
    {
        sut.start_move(Player::Direction::DOWN);

        auto* physics = sut.get_component<Physics>();
        physics->set_position(initial_position_);
        physics->set_move_speed(speed_);

        entity_->add_component<Physics>();
        Game::instance().entity_collection().push_back(std::unique_ptr<Entity>(entity_));

        CollisionMock::clear_defaults();
    }
};
const sf::Vector2f Collisions::initial_position_ = {10.f, 20.f};
const float Collisions::speed_                   = 10.f;

TEST_F(Collisions, WhenSanityCheckFails_DoesFullUpdate)
{
    EXPECT_CALL(collision_, sanity_check(Ref(sut), Ref(*entity_))).WillOnce(Return(false));
    sut.update(sf::seconds(0.5f));

    EXPECT_EQ(sf::Vector2f(10.f, 25.f), sut.get_component<Physics>()->get_position());
}

TEST_F(Collisions, WhenBroadTestPasses_DoesFullUpdate)
{
    ON_CALL(collision_, sanity_check(Ref(sut), Ref(*entity_))).WillByDefault(Return(true));

    EXPECT_CALL(collision_, broad_test(_, _)).WillOnce(Return(false));
    sut.update(sf::seconds(0.5f));

    EXPECT_EQ(sf::Vector2f(10.f, 25.f), sut.get_component<Physics>()->get_position());
}

TEST_F(Collisions, WhenNarrowTestPasses_DoesFullUpdate)
{
    ON_CALL(collision_, sanity_check(Ref(sut), Ref(*entity_))).WillByDefault(Return(true));
    ON_CALL(collision_, broad_test(_, _)).WillByDefault(Return(true));

    EXPECT_CALL(collision_, narrow_test(_, _)).WillOnce(Return(1.f));
    sut.update(sf::seconds(0.5f));

    EXPECT_EQ(sf::Vector2f(10.f, 25.f), sut.get_component<Physics>()->get_position());
}

TEST_F(Collisions, WhenNarrowTestGivesGT0LT1_DoesPartialUpdate)
{
    ON_CALL(collision_, sanity_check(Ref(sut), Ref(*entity_))).WillByDefault(Return(true));
    ON_CALL(collision_, broad_test(_, _)).WillByDefault(Return(true));

    EXPECT_CALL(collision_, narrow_test(_, _)).WillOnce(Return(0.5f));

    sut.update(sf::seconds(0.5f));

    EXPECT_EQ(sf::Vector2f(10.f, 22.5f), sut.get_component<Physics>()->get_position());
}

TEST_F(Collisions, WhenNarrowTestGives0_MovesTheOtherEntityByPenetrationAmount)
{
    ON_CALL(collision_, sanity_check(Ref(sut), Ref(*entity_))).WillByDefault(Return(true));
    ON_CALL(collision_, broad_test(_, _)).WillByDefault(Return(true));

    EXPECT_CALL(collision_, narrow_test(_, _)).WillOnce(Return(0.f));
    EXPECT_CALL(collision_, get_penetration(_, _)).WillOnce(Return(sf::Vector2f(1.f, 2.f)));

    sut.update(sf::seconds(0.5f));
    EXPECT_EQ(sf::Vector2f(1.f, 2.f), entity_->get_component<Physics>()->get_position());
}

class MultiCollisions : public TestablePlayer
{
protected:
    static const sf::Vector2f initial_position_;
    static const float speed_;

    CollisionMock& collision_;
    EntityMock* entity1_;
    EntityMock* entity2_;

    MultiCollisions() :
        collision_(CollisionMock::instance()),
        entity1_(new EntityMock()),
        entity2_(new EntityMock())
    {
        sut.start_move(Player::Direction::DOWN);

        auto* physics = sut.get_component<Physics>();
        physics->set_position(initial_position_);
        physics->set_move_speed(speed_);

        entity1_->add_component<Physics>();
        entity1_->get_component<Physics>()->set_position(sf::Vector2f(1.f, 2.f));
        Game::instance().entity_collection().push_back(std::unique_ptr<Entity>(entity1_));

        entity2_->add_component<Physics>();
        entity2_->get_component<Physics>()->set_position(sf::Vector2f(11.f, 12.f));
        Game::instance().entity_collection().push_back(std::unique_ptr<Entity>(entity2_));

        CollisionMock::clear_defaults();
    }
};
const sf::Vector2f MultiCollisions::initial_position_ = {10.f, 20.f};
const float MultiCollisions::speed_                   = 10.f;

TEST_F(MultiCollisions, WhenBothFailNarrowTest_UsesTheSmallerTimeDelta1)
{
    ON_CALL(collision_, sanity_check(_, _)).WillByDefault(Return(true));
    ON_CALL(collision_, broad_test(_, _)).WillByDefault(Return(true));

    auto entity1_box = entity1_->get_component<Physics>()->get_box();
    auto entity2_box = entity2_->get_component<Physics>()->get_box();
    EXPECT_CALL(collision_, narrow_test(_, entity1_box)).WillOnce(Return(0.8f));
    EXPECT_CALL(collision_, narrow_test(_, entity2_box)).WillOnce(Return(0.5f));

    sut.update(sf::seconds(1.f));
    EXPECT_EQ(sf::Vector2f(10.f, 25.f), sut.get_component<Physics>()->get_position());
}

TEST_F(MultiCollisions, WhenBothFailNarrowTest_UsesTheSmallerTimeDelta2)
{
    ON_CALL(collision_, sanity_check(_, _)).WillByDefault(Return(true));
    ON_CALL(collision_, broad_test(_, _)).WillByDefault(Return(true));

    auto entity1_box = entity1_->get_component<Physics>()->get_box();
    auto entity2_box = entity2_->get_component<Physics>()->get_box();
    EXPECT_CALL(collision_, narrow_test(_, entity1_box)).WillOnce(Return(0.8f));
    EXPECT_CALL(collision_, narrow_test(_, entity2_box)).WillOnce(Return(0.5f));

    sut.update(sf::seconds(2.f));
    EXPECT_EQ(sf::Vector2f(10.f, 30.f), sut.get_component<Physics>()->get_position());
}

TEST_F(MultiCollisions, WhenBothFailNarrowTest_AndOneReturns0_DoesNotMoveThePlayer)
{
    ON_CALL(collision_, sanity_check(_, _)).WillByDefault(Return(true));
    ON_CALL(collision_, broad_test(_, _)).WillByDefault(Return(true));

    auto entity1_box = entity1_->get_component<Physics>()->get_box();
    auto entity2_box = entity2_->get_component<Physics>()->get_box();
    EXPECT_CALL(collision_, narrow_test(_, entity1_box)).WillOnce(Return(0.8f));
    EXPECT_CALL(collision_, narrow_test(_, entity2_box)).WillOnce(Return(0.0f));
    EXPECT_CALL(collision_, get_penetration(_, entity2_box)).
        WillOnce(Return(sf::Vector2f(1.f, 2.f)));

    sut.update(sf::seconds(1.f));
    EXPECT_EQ(sf::Vector2f(12.f, 14.f), entity2_->get_component<Physics>()->get_position());
    EXPECT_EQ(sf::Vector2f(10.f, 20.f), sut.get_component<Physics>()->get_position());
}
