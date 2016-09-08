#include <gtest.h>
#include <gmock.h>

#include "health_bar.hpp"
#include "gun.hpp"

using namespace testing;

class TestableHealthBar : public Test
{
protected:
    static const sf::Vector2f dimensions_;
    static const sf::Vector2f position_;

    HealthBar sut;

    TestableHealthBar() :
        sut()
    {
        sut.set_dimensions(dimensions_);
        sut.set_position(position_);
    }
};
const sf::Vector2f TestableHealthBar::dimensions_ = {100.f, 10.f};
const sf::Vector2f TestableHealthBar::position_   = {60.f, 6.f}; ///< min corner at <10.f, 1.f>

class PercentFilled : public TestableHealthBar { };

TEST_F(PercentFilled, InsideAndOutsideValidRange)
{
    sut.set_filled(1.1f);
    EXPECT_EQ(1.f, sut.get_filled());

    sut.set_filled(1.f);
    EXPECT_EQ(1.f, sut.get_filled());

    sut.set_filled(0.f);
    EXPECT_EQ(0.f, sut.get_filled());

    sut.set_filled(-0.1f);
    EXPECT_EQ(0.f, sut.get_filled());
}

class Render : public TestableHealthBar
{
protected:
    static constexpr float full_health_     = 1.f;
    static constexpr float half_health_     = 0.5f;
    static constexpr float zero_health_     = 0.f;
};

TEST_F(Render, ZeroHealth_OutlineOnly)
{
    sut.set_filled(zero_health_);
    sut.render();
    auto renderings = sut.get_renderings();
    ASSERT_EQ(2, renderings.size());

    const auto& outline = *static_cast<const sf::RectangleShape*>(renderings[0]);

    EXPECT_EQ(sf::Vector2f(60.f, 6.f), outline.getPosition());
    EXPECT_EQ(sf::Vector2f(50.f, 5.f), outline.getOrigin());
    EXPECT_EQ(sf::Vector2f(100.f, 10.f), outline.getSize());
    EXPECT_LT(outline.getOutlineThickness(), 5.f);
    EXPECT_EQ(sf::Color::White, outline.getFillColor());

    const auto& filled = *static_cast<const sf::RectangleShape*>(renderings[1]);

    EXPECT_EQ(sf::Vector2f(10.f, 1.f), filled.getPosition());
    EXPECT_EQ(sf::Vector2f(0.f, 0.f), filled.getOrigin());
    EXPECT_EQ(sf::Vector2f(0.f, 10.f), filled.getSize());

    EXPECT_EQ(outline.getOutlineColor(), filled.getFillColor());
}

TEST_F(Render, FullHealth_CompletelyFilled)
{
    sut.set_filled(full_health_);
    sut.render();
    auto renderings = sut.get_renderings();
    ASSERT_EQ(2, renderings.size());

    const auto& outline = *static_cast<const sf::RectangleShape*>(renderings[0]);

    EXPECT_EQ(sf::Vector2f(60.f, 6.f), outline.getPosition());
    EXPECT_EQ(sf::Vector2f(50.f, 5.f), outline.getOrigin());
    EXPECT_EQ(sf::Vector2f(100.f, 10.f), outline.getSize());
    EXPECT_LT(outline.getOutlineThickness(), 5.f);
    EXPECT_EQ(sf::Color::White, outline.getFillColor());

    const auto& filled = *static_cast<const sf::RectangleShape*>(renderings[1]);

    EXPECT_EQ(sf::Vector2f(10.f, 1.f), filled.getPosition());
    EXPECT_EQ(sf::Vector2f(0.f, 0.f), filled.getOrigin());
    EXPECT_EQ(sf::Vector2f(100.f, 10.f), filled.getSize());

    EXPECT_EQ(outline.getOutlineColor(), filled.getFillColor());
}

TEST_F(Render, HalfHealth_PartiallyFilledPartiallyOutline)
{
    sut.set_filled(half_health_);
    sut.render();
    auto renderings = sut.get_renderings();
    ASSERT_EQ(2, renderings.size());

    const auto& outline = *static_cast<const sf::RectangleShape*>(renderings[0]);

    EXPECT_EQ(sf::Vector2f(60.f, 6.f), outline.getPosition());
    EXPECT_EQ(sf::Vector2f(50.f, 5.f), outline.getOrigin());
    EXPECT_EQ(sf::Vector2f(100.f, 10.f), outline.getSize());
    EXPECT_LT(outline.getOutlineThickness(), 5.f);
    EXPECT_EQ(sf::Color::White, outline.getFillColor());

    const auto& filled = *static_cast<const sf::RectangleShape*>(renderings[1]);

    EXPECT_EQ(sf::Vector2f(10.f, 1.f), filled.getPosition());
    EXPECT_EQ(sf::Vector2f(0.f, 0.f), filled.getOrigin());
    EXPECT_EQ(sf::Vector2f(50.f, 10.f), filled.getSize());

    EXPECT_EQ(outline.getOutlineColor(), filled.getFillColor());
}
