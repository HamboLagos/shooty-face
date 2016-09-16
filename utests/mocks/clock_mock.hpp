#include "clock.hpp"

class ClockMock : public ClockIF
{
public:
    ClockMock() = default;
    virtual ~ClockMock() = default;

    MOCK_CONST_METHOD0(getElapsedTime, sf::Time(void));
    MOCK_METHOD0(restart, sf::Time(void));
};
