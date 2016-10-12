#pragma once

#include <gmock.h>

#include "rate_limit.hpp"

class RateLimitMock : public RateLimitIF
{
public:
    RateLimitMock() = default;
    virtual ~RateLimitMock() = default;

    MOCK_CONST_METHOD0(check, bool());
    MOCK_METHOD0(renew, void());
};
