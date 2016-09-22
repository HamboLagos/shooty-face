#pragma once

#include <gmock.h>

#include "projectile.hpp"
#include "components/physics.hpp"

class ProjectileMock : public Projectile
{
public:
    ProjectileMock() { add_component<Physics>(); }
    virtual ~ProjectileMock() = default;

    MOCK_METHOD0(fire, void(void));
    MOCK_METHOD1(update, void(sf::Time));
    MOCK_METHOD0(render, const Renderings(void));
};

class AmmunitionMock : public Ammunition
{
public:
    AmmunitionMock() = default;
    virtual ~AmmunitionMock() = default;

    MOCK_METHOD0(create_projectile, Projectile*(void));
    MOCK_METHOD0(reload, void(void));
};
