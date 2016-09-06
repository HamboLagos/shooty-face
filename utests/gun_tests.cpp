#include <gtest.h>
#include <gmock.h>

#include "stub_entity.hpp"
#include "gun.hpp"

using namespace testing;
using Ammunition = Gun::Ammunition;

class TestableGun : public Test
{
protected:
    StubEntity gun_operator_;
    Gun sut;

    TestableGun() :
        sut(gun_operator_)
    { }
};

class FiringBullets : public TestableGun
{
protected:
    static const sf::Vector2f initial_position_;
    static const sf::Vector2f target_;
    static const sf::Vector2f dimensions_;

    FiringBullets()
    {
        gun_operator_.set_position(initial_position_);
    }
};
const sf::Vector2f FiringBullets::initial_position_ = {10.f, 20.f};
const sf::Vector2f FiringBullets::target_           = {20.f, 40.f};
const sf::Vector2f FiringBullets::dimensions_       = {10.f, 10.f};

TEST_F(FiringBullets, ProxyToBulletProjectile)
{
    sut.set_ammunition(Ammunition::Bullet);
    EXPECT_EQ(Ammunition::Bullet, sut.get_ammunition());

    sut.fire(target_);
    auto& actual = static_cast<Bullet&>(*sut.get_last_projectile());

    Bullet expected(initial_position_, target_, Gun::BULLET_SPEED);
    expected.set_dimensions(dimensions_);
    expected.fire();

    EXPECT_EQ(expected, actual);
}
