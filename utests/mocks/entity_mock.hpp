#include <gmock/gmock.h>

#include "entity.hpp"

class EntityMock : public Entity
{
public:
    MOCK_METHOD1(update, void(sf::Time));
};
