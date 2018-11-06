#include "firmware_handler.hpp"

#include <algorithm>
#include <memory>

#include <gtest/gtest.h>

namespace blobs
{

TEST(FirmwareHandlerTest, CreateEmptyListVerifyFails)
{
    auto handler = FirmwareBlobHandler::CreateFirmwareBlobHandler({}, 0);
    EXPECT_EQ(handler, nullptr);
}
TEST(FirmwareHandlerTest, CreateEmptyListVerifyHasHash)
{
    auto handler = FirmwareBlobHandler::CreateFirmwareBlobHandler({"asdf"}, 0);
    auto result = handler->getBlobIds();
    EXPECT_EQ(2, result.size());
    EXPECT_EQ(2, std::count(result.begin(), result.end(), "asdf") +
                     std::count(result.begin(), result.end(),
                                FirmwareBlobHandler::hashBlobID));
}
} // namespace blobs