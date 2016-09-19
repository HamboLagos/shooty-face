#include <gtest.h>
#include <gmock.h>

#include "enemy.hpp"

#include "components/AI.hpp"
#include "components/graphics.hpp"
#include "components/health.hpp"
#include "components/physics.hpp"
#include "mocks/collision_mock.hpp"

using namespace testing;

class TestableEnemy : public Test
{
protected:
    Enemy sut;
};

class Components : public TestableEnemy { };

TEST_F(Components, ExistenceThereof)
{
    EXPECT_TRUE(sut.has_component<Physics>());
    EXPECT_TRUE(sut.has_component<Health>());
    EXPECT_TRUE(sut.has_component<Graphics>());
    EXPECT_TRUE(sut.has_component<AI>());
}

TEST_F(Components, Physics_SetsDefaults)
{
    auto* physics = sut.get_component<Physics>();
    EXPECT_FLOAT_EQ(Enemy::SPEED, physics->get_move_speed());
}

TEST_F(Components, Graphics_ReturnsARectangleForThisEnemy)
{

    auto* physics = sut.get_component<Physics>();

    physics->set_position({100.1f, 200.2f});
    physics->set_dimensions({10.1f, 20.2f});

    const auto renderings = sut.get_component<Graphics>()->render();
    const auto* graphic = static_cast<const sf::RectangleShape*>(renderings[0]);

    EXPECT_EQ(sf::Vector2f(100.f, 200.f), graphic->getPosition());
    EXPECT_EQ(sf::Vector2f(10.1f, 20.2f), graphic->getSize());
}

/// TODO
class Render : public TestableEnemy { };

/// TODO
class Update : public TestableEnemy { };

TEST_F(Update, DelegatesToAI)
{
    // if dead, no delegation
    // else, delegates to AI
}
