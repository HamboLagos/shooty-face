#include <gtest.h>
#include <gmock.h>

#include "rate_limit.hpp"

#include "mocks/clock_mock.hpp"

using namespace testing;

class TestableRateLimit : public Test
{
protected:
    static constexpr float RATE = 10.f;
    static constexpr float PERIOD = 1.f/RATE;

    NiceMock<ClockMock>* clock_;
    RateLimit sut;

    TestableRateLimit() :
        clock_(new NiceMock<ClockMock>),
        sut(RATE, std::unique_ptr<ClockIF>(clock_))
    { }
};

class Check: public TestableRateLimit { };

TEST_F(Check, DelegatesToUnderlyingClock)
{
    EXPECT_CALL(*clock_, getElapsedTime()).WillOnce(Return(sf::seconds(PERIOD/2.f)));
    EXPECT_FALSE(sut.check());

    EXPECT_CALL(*clock_, getElapsedTime()).WillOnce(Return(sf::seconds(PERIOD)));
    EXPECT_TRUE(sut.check());

    EXPECT_CALL(*clock_, getElapsedTime()).WillOnce(Return(sf::seconds(PERIOD*2.f)));
    EXPECT_TRUE(sut.check());
}

class Renew : public TestableRateLimit { };

TEST_F(Renew, RestartsTheInternalClock)
{
    EXPECT_CALL(*clock_, restart()).Times(1);
    sut.renew();
}
