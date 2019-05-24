/**
 * The goal of these tests is to verify the behavior of all blob commands given
 * the current state is notYetStarted.  The initial state.
 */
#include "firmware_handler.hpp"
#include "firmware_unittest.hpp"

#include <cstdint>
#include <string>
#include <vector>

#include <gtest/gtest.h>

namespace ipmi_flash
{
namespace
{

using ::testing::Return;
using ::testing::UnorderedElementsAreArray;

class FirmwareHandlerNotYetStartedTest : public IpmiOnlyFirmwareStaticTest
{
  protected:
    std::uint16_t session = 1;
    std::uint16_t flags =
        blobs::OpenFlags::write | FirmwareBlobHandler::UpdateFlags::ipmi;
};

/*
 * There are the following calls (parameters may vary):
 * Note: you cannot have a session yet, so only commands that don't take a
 * session parameter are valid. Once you open() from this state, we will vary
 * you transition out of this state (ensuring the above is true). Technically
 * the firmware handler receives the session number with open(), but the blob
 * manager is providing this normally.
 *
 * canHandleBlob
 * getBlobIds
 * deleteBlob
 * stat
 * open
 *
 * canHandleBlob is just a count check (or something similar) against what is
 * returned by getBlobIds.  It is tested in firmware_canhandle_unittest
 */

TEST_F(FirmwareHandlerNotYetStartedTest, GetBlobListValidateListContents)
{
    /* TODO: Presently, /flash/verify is present from the beginning, however,
     * this is going to change to simplify handling of open().
     */
    std::vector<std::string> expectedBlobs = {staticLayoutBlobId, hashBlobId,
                                              verifyBlobId};

    EXPECT_THAT(handler->getBlobIds(),
                UnorderedElementsAreArray(expectedBlobs));
}

/* TODO: Try deleting some blobs -- in this state it should just return failure,
 * but it's not yet implemented
 */

/* stat(blob_id) */
TEST_F(FirmwareHandlerNotYetStartedTest, StatEachBlobIdVerifyResults)
{
    /* In this original state, calling stat() on the blob ids will return the
     * transported supported.
     */
    blobs::BlobMeta expected;
    expected.blobState = FirmwareBlobHandler::UpdateFlags::ipmi;
    expected.size = 0;

    /* TODO: note, once verifyblobid isn't present in this state we can use
     * getblobids()'s output as input
     */
    std::vector<std::string> testBlobs = {staticLayoutBlobId, hashBlobId};
    for (const auto& blob : testBlobs)
    {
        blobs::BlobMeta meta = {};
        EXPECT_TRUE(handler->stat(blob, &meta));
        EXPECT_EQ(expected, meta);
    }
}

/* open(each blob id) (verifyblobid will no longer be available at this state.
 */
TEST_F(FirmwareHandlerNotYetStartedTest, OpenStaticImageFileVerifyStateChange)
{
    auto realHandler = dynamic_cast<FirmwareBlobHandler*>(handler.get());

    EXPECT_CALL(imageMock, open(staticLayoutBlobId)).WillOnce(Return(true));

    EXPECT_TRUE(handler->open(session, flags, staticLayoutBlobId));
    EXPECT_EQ(FirmwareBlobHandler::UpdateState::uploadInProgress,
              realHandler->getCurrentState());
}

TEST_F(FirmwareHandlerNotYetStartedTest, OpenHashFileVerifyStateChange)
{
    auto realHandler = dynamic_cast<FirmwareBlobHandler*>(handler.get());

    EXPECT_CALL(imageMock, open(hashBlobId)).WillOnce(Return(true));

    EXPECT_TRUE(handler->open(session, flags, hashBlobId));
    EXPECT_EQ(FirmwareBlobHandler::UpdateState::uploadInProgress,
              realHandler->getCurrentState());
}

} // namespace
} // namespace ipmi_flash