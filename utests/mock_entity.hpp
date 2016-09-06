#include <gmock/gmock.h>

#include "entity.hpp"

class MockEntity : public Entity
{
public:
    MOCK_METHOD1(update, void(sf::Time));
    MOCK_METHOD0(render, const sf::Drawable&());
};
