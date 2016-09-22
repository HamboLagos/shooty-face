#include <gtest.h>
#include <gmock.h>

#include "gun.hpp"

#include "mocks/entity_mock.hpp"
#include "mocks/projectile_mock.hpp"

#include <SFML/Graphics/RectangleShape.hpp>

using namespace testing;

class TestableGun : public Test
{
protected:
    AmmunitionMock* ammunition_;
    EntityMock operator_;
    Gun sut;

    TestableGun() :
        ammunition_(new AmmunitionMock),
        sut(operator_)
    {
        operator_.add_component<Physics>();
        sut.set_ammunition(std::unique_ptr<Ammunition>(ammunition_));
    }

    Gun::Magazine& sut_get_magazine() { return sut.magazine_; }
};

class Components : public TestableGun { };

TEST_F(Components, ExistenceThereof)
{
    EXPECT_TRUE(sut.has_component<Graphics>());
}

TEST_F(Components, Graphics_DelegatesToRenderForAllLiveProjectiles)
{
    ProjectileMock* projectile1 = new ProjectileMock();
    sut_get_magazine().push_back(std::unique_ptr<Projectile>(projectile1));

    Graphics::Renderings renderings1;
    sf::RectangleShape rendering11; renderings1.push_back(&rendering11);
    sf::RectangleShape rendering12; renderings1.push_back(&rendering12);
    EXPECT_CALL(*projectile1, render()).WillOnce(Return(renderings1));

    ProjectileMock* projectile2 = new ProjectileMock();
    sut_get_magazine().push_back(std::unique_ptr<Projectile>(projectile2));

    Graphics::Renderings renderings2;
    sf::RectangleShape rendering21; renderings2.push_back(&rendering21);
    sf::RectangleShape rendering22; renderings2.push_back(&rendering22);
    EXPECT_CALL(*projectile2, render()).WillOnce(Return(renderings2));

    const auto& renderings = sut.render();
    EXPECT_EQ(4, renderings.size());
    EXPECT_EQ(&rendering11, renderings[0]);
    EXPECT_EQ(&rendering12, renderings[1]);
    EXPECT_EQ(&rendering21, renderings[2]);
    EXPECT_EQ(&rendering22, renderings[3]);
}

class Fire : public TestableGun
{
protected:
    static const sf::Vector2f initial_position_;
    static const sf::Vector2f target_;

    ProjectileMock* projectile_;

    Fire() :
        projectile_(new ProjectileMock)
    {
        operator_.get_component<Physics>()->set_position(initial_position_);
    }
};
const sf::Vector2f Fire::initial_position_ = {10.f, 20.f};
const sf::Vector2f Fire::target_           = {20.f, 40.f};

TEST_F(Fire, CreatesProjectileFromAmmunition_FiresProjectile_SavesProjectileInMagazine)
{
    EXPECT_CALL(*ammunition_, create_projectile()).WillOnce(Return(projectile_));
    EXPECT_CALL(*projectile_, fire()).Times(Exactly(1));

    sut.fire(target_);

    EXPECT_EQ(initial_position_, projectile_->get_component<Physics>()->get_position());
    EXPECT_EQ(target_, projectile_->get_target());
    EXPECT_EQ(1, sut_get_magazine().size());
    EXPECT_EQ(projectile_, sut_get_magazine()[0].get());

    // returning nullptr from ammunition has no effect on gun
    EXPECT_CALL(*ammunition_, create_projectile()).WillOnce(Return(nullptr));
    sut.fire(target_);
    EXPECT_EQ(1, sut_get_magazine().size());
}

class Reload : public TestableGun { };

TEST_F(Reload, ProxiesToAmmunition)
{
    EXPECT_CALL(*ammunition_, reload()).Times(1);
    sut.reload();
}

class Update : public TestableGun { };

TEST_F(Update, RemovesAllDeadProjectiles)
{
    ProjectileMock* dead1 = new ProjectileMock(); dead1->kill();
    sut_get_magazine().push_back(std::unique_ptr<Projectile>(dead1));

    NiceMock<ProjectileMock>* alive1 = new NiceMock<ProjectileMock>();
    sut_get_magazine().push_back(std::unique_ptr<Projectile>(alive1));

    ProjectileMock* dead2 = new ProjectileMock(); dead2->kill();
    sut_get_magazine().push_back(std::unique_ptr<Projectile>(dead2));

    NiceMock<ProjectileMock>* alive2 = new NiceMock<ProjectileMock>();
    sut_get_magazine().push_back(std::unique_ptr<Projectile>(alive2));

    sut.update(sf::Time::Zero);

    EXPECT_EQ(2, sut_get_magazine().size());
    EXPECT_EQ(alive1, sut_get_magazine()[0].get());
    EXPECT_EQ(alive2, sut_get_magazine()[1].get());
}

TEST_F(Update, ProxiesUpdateToEachLiveProjectile)
{
    ProjectileMock* alive1 = new ProjectileMock();
    sut_get_magazine().push_back(std::unique_ptr<Projectile>(alive1));
    EXPECT_CALL(*alive1, update(sf::seconds(0.5f)));

    ProjectileMock* alive2 = new ProjectileMock();
    sut_get_magazine().push_back(std::unique_ptr<Projectile>(alive2));
    EXPECT_CALL(*alive2, update(sf::seconds(0.5f)));

    sut.update(sf::seconds(0.5f));
}
