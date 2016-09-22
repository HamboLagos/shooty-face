#pragma once

#include <gmock.h>

#include "components/AI.hpp"

class AIMock : public AI
{
public:
    AIMock(Entity& entity) :
        AI(entity)
    { }
    virtual ~AIMock() = default;

    MOCK_METHOD1(update,  sf::Time(sf::Time));
};
