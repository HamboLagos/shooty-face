#include <gtest.h>
#include <gmock.h>

#include "barrier.hpp"

#include "components/physics.hpp"

using namespace testing;

class TestableBarrier : public Test
{
protected:
    Barrier sut;
};

class Components : public TestableBarrier { };

TEST_F(Components, ExistenceThereof)
{
    EXPECT_TRUE(sut.has_component<Physics>());
    EXPECT_TRUE(sut.has_component<Graphics>());
}

TEST_F(Components, Physics)
{
    EXPECT_TRUE(sut.get_component<Physics>()->is_static());
}

TEST_F(Components, Render_ReturnsARectangleForThisBarrier)
{
    auto* physics = sut.get_component<Physics>();

    physics->set_position({100.1f, 200.2f});
    physics->set_dimensions({10.1f, 20.2f});

    const auto renderings = sut.get_component<Graphics>()->render();
    const auto* graphic = static_cast<const sf::RectangleShape*>(renderings[0]);

    EXPECT_EQ(sf::Vector2f(100.f, 200.f), graphic->getPosition());
    EXPECT_EQ(sf::Vector2f(10.1f, 20.2f), graphic->getSize());
}
