#include "data_mock.hpp"
#include "firmware_handler.hpp"

#include <phosphor-logging/test/sdjournal_mock.hpp>
#include <sdbusplus/test/sdbus_mock.hpp>

#include <gtest/gtest.h>

using namespace phosphor::logging;

namespace blobs
{
using ::testing::Return;
using ::testing::StrEq;
using ::testing::StrictMock;

TEST(FirmwareHandlerBlobTest, VerifyFirmareCounts)
{
    /* Verify the firmware count must be greater than zero. */

    DataHandlerMock dataMock;
    StrictMock<SdJournalMock> journalMock;
    SwapJouralHandler(&journalMock);

    std::vector<HandlerPack> blobs;

    std::vector<DataHandlerPack> data = {
        {FirmwareBlobHandler::UpdateFlags::ipmi, nullptr},
        {FirmwareBlobHandler::UpdateFlags::lpc, &dataMock},
    };

    sdbusplus::SdBusMock sdbus_mock;
    auto bus_mock = sdbusplus::get_mocked_new(&sdbus_mock);

    EXPECT_CALL(journalMock, journal_send_call(StrEq("PRIORITY=%d")))
        .WillOnce(Return(0));

    auto handler = FirmwareBlobHandler::CreateFirmwareBlobHandler(
        std::move(bus_mock), blobs, data);

    EXPECT_EQ(handler, nullptr);
}

} // namespace blobs