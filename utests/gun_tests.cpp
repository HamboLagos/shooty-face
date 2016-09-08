#include <gtest.h>
#include <gmock.h>

#include "gun.hpp"
#include "bullet.hpp"

class MockEntity : public Entity
{
public:
    MOCK_METHOD1(update, void(sf::Time));
};

class MockAmmunition : public Ammunition
{
public:
    MOCK_METHOD0(create_projectile, Projectile*());
};

class MockProjectile : public Projectile
{
public:
    MOCK_METHOD0(fire, void());
    MOCK_METHOD1(update, void(sf::Time));
    MOCK_METHOD0(render, void());
};

using namespace testing;

class TestableGun : public Test
{
protected:
    MockEntity operator_;
    Gun sut;

    TestableGun() :
        sut(operator_)
    { }
};

class FiringBullets : public TestableGun
{
protected:
    static const sf::Vector2f initial_position_;
    static const sf::Vector2f target_;

    MockAmmunition* ammunition_;
    MockProjectile* projectile_;

    FiringBullets() :
        ammunition_(new MockAmmunition),
        projectile_(new MockProjectile)
    {
        operator_.set_position(initial_position_);
    }
};
const sf::Vector2f FiringBullets::initial_position_ = {10.f, 20.f};
const sf::Vector2f FiringBullets::target_           = {20.f, 40.f};

TEST_F(FiringBullets, ProxyToBulletProjectile)
{
    sut.set_ammunition(std::unique_ptr<MockAmmunition>(ammunition_));
    ammunition_ = static_cast<MockAmmunition*>(sut.get_ammunition());

    EXPECT_CALL(*ammunition_, create_projectile())
        .WillOnce(Return(projectile_));

    EXPECT_CALL(*projectile_, fire())
        .Times(Exactly(1));

    sut.fire(target_);

    EXPECT_EQ(initial_position_, projectile_->get_position());
    EXPECT_EQ(target_, projectile_->get_target());
    EXPECT_TRUE(projectile_->is_alive());

    // Google mock does not play nice with smart pointers. Oh well.
    Mock::VerifyAndClearExpectations(ammunition_);
    Mock::AllowLeak(ammunition_);
}
