#include <gtest.h>
#include <gmock.h>

#include "entity.hpp"

class StubEntity : public Entity {
    sf::Time update(sf::Time elapsed) override { return sf::Time::Zero; }
};

using namespace testing;

class TestableEntity : public Test
{
protected:
    StubEntity sut;
};

class Alive : public TestableEntity { };

TEST_F(Alive, OnConstruction_IsAlive)
{
    EXPECT_TRUE(sut.is_alive());
}

TEST_F(Alive, KillingTheEntity)
{
    EXPECT_TRUE(sut.is_alive());
    EXPECT_FALSE(sut.is_dead());

    sut.kill();
    EXPECT_TRUE(sut.is_dead());
    EXPECT_FALSE(sut.is_alive());

    sut.set_alive(true);
    EXPECT_TRUE(sut.is_alive());
    EXPECT_FALSE(sut.is_dead());

    sut.set_alive(false);
    EXPECT_TRUE(sut.is_dead());
    EXPECT_FALSE(sut.is_alive());
}

class Components : public TestableEntity { };

TEST_F(Components, AddComponentOverwritesExistingComponent)
{
    EXPECT_FALSE(sut.has_component<Component>());

    auto* component = sut.add_component<Component>();
    EXPECT_TRUE(sut.has_component<Component>());

    auto* other_component = sut.add_component<Component>();
    EXPECT_TRUE(sut.has_component<Component>());
    EXPECT_NE(other_component, component);
}

TEST_F(Components, SetComponentOverwritesExistingComponent)
{
    EXPECT_FALSE(sut.has_component<Component>());

    auto* component = sut.set_component<Component>(std::make_unique<Component>(sut));
    EXPECT_TRUE(sut.has_component<Component>());

    auto* other_component = sut.set_component<Component>(std::make_unique<Component>(sut));
    EXPECT_TRUE(sut.has_component<Component>());
    EXPECT_NE(other_component, component);
}

TEST_F(Components, HasComponentReturnsFalseIfNotSetOrIsNullptr)
{
    EXPECT_FALSE(sut.has_component<Component>());

    sut.add_component<Component>();
    EXPECT_TRUE(sut.has_component<Component>());

    sut.set_component<Component>(std::unique_ptr<Component>(nullptr));
    EXPECT_FALSE(sut.has_component<Component>());
}

TEST_F(Components, GetComponentReturnsNullptrOrTheSetComponent)
{
    EXPECT_FALSE(sut.has_component<Component>());
    EXPECT_EQ(nullptr, sut.get_component<Component>());

    sut.set_component<Component>(std::unique_ptr<Component>(nullptr));
    EXPECT_FALSE(sut.has_component<Component>());
    EXPECT_EQ(nullptr, sut.get_component<Component>());

    auto* component = sut.add_component<Component>();
    EXPECT_TRUE(sut.has_component<Component>());
    EXPECT_EQ(component, sut.get_component<Component>());
}
