#pragma once

#include "ipmi_interface.hpp"

#include <gmock/gmock.h>

class IpmiInterfaceMock : public IpmiInterface
{
  public:
    virtual ~IpmiInterfaceMock() = default;
    MOCK_METHOD1(sendPacket, int(const std::vector<std::uint8_t>&));
};