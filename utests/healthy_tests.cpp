#include <gtest.h>
#include <gmock.h>

#include "healthy.hpp"

using namespace testing;

class MockHealthy : public Healthy
{
public:
    MockHealthy(float initial_health) :
        Healthy(initial_health)
    { }

    MOCK_METHOD1(on_heal, void(float));
    MOCK_METHOD1(on_damage, void(float));
    MOCK_METHOD0(on_death, void());
};

class TestableHealthy : public Test
{
protected:
    static constexpr float base_health_ = 100.f;
    MockHealthy sut;

    TestableHealthy() :
        sut(base_health_)
    { }
};

class Callbacks : public TestableHealthy { };

TEST_F(Callbacks, OnHeal)
{
    float heal_amount = 12.5f;
    EXPECT_CALL(sut, on_heal(heal_amount))
        .Times(Exactly(1));

    sut.heal(heal_amount);
}

TEST_F(Callbacks, OnDamage)
{
    float damage_amount = 12.5f;
    EXPECT_CALL(sut, on_damage(damage_amount))
        .Times(Exactly(1));

    sut.damage(damage_amount);
}

TEST_F(Callbacks, OnDeathAtZeroHealth)
{
    float damage_amount = base_health_;
    {
        InSequence DeathSequence;
        EXPECT_CALL(sut, on_damage(damage_amount))
            .Times(Exactly(1));
        EXPECT_CALL(sut, on_death())
            .Times(Exactly(1));
    }

    sut.damage(damage_amount);
}

TEST_F(Callbacks, OnDeathAtNegativeHealth)
{
    float damage_amount = base_health_ + 10.f;
    {
        InSequence DeathSequence;
        EXPECT_CALL(sut, on_damage(damage_amount))
            .Times(Exactly(1));
        EXPECT_CALL(sut, on_death())
            .Times(Exactly(1));
    }

    sut.damage(damage_amount);
}
