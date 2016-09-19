#pragma once

class AIMock : public AI
{
public:
    AIMock(Entity& entity) :
        AI(entity)
    { }
    virtual ~AIMock() = default;

    MOCK_METHOD1(update, void(float));
};
