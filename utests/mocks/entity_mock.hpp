#pragma once

#include <gmock.h>

#include "entity.hpp"

class EntityMock : public Entity
{
public:
    MOCK_METHOD1(update, sf::Time(sf::Time));
};
