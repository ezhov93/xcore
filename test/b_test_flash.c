#include "ecore/drivers/flash.h"
#include "unity.h"
#include "unity_fixture.h"
#include <time.h>

#include "ecore/waitcounter.h"

#define FLASH_DEFAULT_FLASH_BYTE (0xFF)
#define FLASH_DEFAULT_PARTIAL_PAGE_SIZE (32)
#define FlashDefaultMaxBufferSize (512)

#define FLASH_DEFAULT_DELAY_US (100)
#define FLASH_INVALID_HANDLE (-1)

static unsigned char Instance_ = 0;

static int startOffset_ = 0;

static unsigned char writeBuffer_[FlashDefaultMaxBufferSize];
static unsigned char readBuffer_[FlashDefaultMaxBufferSize];

static const unsigned char FlashWriteROBuffer_[512] = {0xAA};

TEST_GROUP(TestFlash);

TEST_SETUP(TestFlash) {}

TEST_TEAR_DOWN(TestFlash) {}

static void writeDummyData_(Flash flash, int offset, int size) {
  int result;
  int i;
  int j;
  for (i = 0; i < size; i += FlashDefaultMaxBufferSize) {
    int remainingSize = ((i + FlashDefaultMaxBufferSize) > size)
                            ? (size - i)
                            : FlashDefaultMaxBufferSize;
    if (remainingSize > 0) {
      for (j = 0; j < remainingSize; j++)
        writeBuffer_[j] = j;

      result =
          Flash_writeSync(flash, offset + i, &writeBuffer_[0], remainingSize);
      TEST_ASSERT_EQUAL(FlashSuccess, result);
    }
  }
}

static void readVerifyDummyData_(Flash flash, int offset, int size) {
  int result;
  int i;
  int j;
  for (i = 0; i < size; i += FlashDefaultMaxBufferSize) {
    int remainingSize = ((i + FlashDefaultMaxBufferSize) > size)
                            ? (size - i)
                            : FlashDefaultMaxBufferSize;
    if (remainingSize > 0) {
      result =
          Flash_readSync(flash, offset + i, &readBuffer_[0], remainingSize);
      TEST_ASSERT_EQUAL(FlashSuccess, result);
      for (j = 0; j < remainingSize; j++) {
        if (readBuffer_[j] != (j & 0xFF)) {
          TEST_ASSERT_MESSAGE(0, "Data was not written");
        }
      }
    }
  }
}

static void readVerifyErased_(Flash flash, int offset, int size) {
  int result;
  int i;
  int j;
  for (i = 0; i < size; i += FlashDefaultMaxBufferSize) {
    int remainingSize = ((i + FlashDefaultMaxBufferSize) > size)
                            ? (size - i)
                            : FlashDefaultMaxBufferSize;
    if (remainingSize > 0) {
      result =
          Flash_readSync(flash, offset + i, &readBuffer_[0], remainingSize);
      TEST_ASSERT_EQUAL(FlashSuccess, result);
			
      for (j = 0; j < remainingSize; j++) {
        if (readBuffer_[j] != FLASH_DEFAULT_FLASH_BYTE) {
          TEST_ASSERT_MESSAGE(0, "Data was NOT erased");
        }
      }
    }
  }
}

static void writeReadVerify_(Flash flash, int offset, int size) {
  writeDummyData_(flash, offset, size);
  readVerifyDummyData_(flash, offset, size);
}
TEST_GROUP_RUNNER(TestFlash) {
  RUN_TEST_CASE(TestFlash, FlashOpenClose);
  RUN_TEST_CASE(TestFlash, FlashReadInfo);
  RUN_TEST_CASE(TestFlash, FlashEraseSector);
  RUN_TEST_CASE(TestFlash, FlashEraseMultipleSectors);
  RUN_TEST_CASE(TestFlash, FlashEraseFlashBlocks);
  RUN_TEST_CASE(TestFlash, FlashEraseFlashBlocksUnAlignedAddress);
  RUN_TEST_CASE(TestFlash, FlashEraseFlashBlocksUnAlignedSize);
  RUN_TEST_CASE(TestFlash, FlashEraseUnAlignedAddress);
  RUN_TEST_CASE(TestFlash, FlashEraseUnAlignedSize);
  RUN_TEST_CASE(TestFlash, FlashWritePartialPage);
  RUN_TEST_CASE(TestFlash, FlashWritePage);
  RUN_TEST_CASE(TestFlash, FlashWriteSector);
  RUN_TEST_CASE(TestFlash, FlashWriteAcrossSectors);
  RUN_TEST_CASE(TestFlash, FlashEraseInProgressRead);
  RUN_TEST_CASE(TestFlash, FlashEraseInProgressWrite);
  RUN_TEST_CASE(TestFlash, FlashWriteProtectWriteFailure);
  RUN_TEST_CASE(TestFlash, FlashWriteProtectEraseFailure);
  RUN_TEST_CASE(TestFlash, FlashWriteEraseReadCycle);
  RUN_TEST_CASE(TestFlash, FlashWritePageFromFlash);
  RUN_TEST_CASE(TestFlash, FlashOpenCloseFuzz);
  RUN_TEST_CASE(TestFlash, FlashGetInfoFuzz);
  RUN_TEST_CASE(TestFlash, FlashIoctlFuzz);
  RUN_TEST_CASE(TestFlash, FlashWriteFuzz);
  RUN_TEST_CASE(TestFlash, FlashReadFuzz);
}

TEST(TestFlash, FlashOpenClose) {
  Flash flash;
  int result;
	
  flash = flash_open(Instance_);
  TEST_ASSERT_NOT_EQUAL(NULL, flash);
	
  result = flash_close(flash);
  TEST_ASSERT_EQUAL(FlashSuccess, result);
}

TEST(TestFlash, FlashReadInfo) {
  Flash flash;
  FlashInfo *info;
  int result;
  flash = flash_open(Instance_);
  TEST_ASSERT_NOT_EQUAL(NULL, flash);

  if (TEST_PROTECT()) {
    info = flash_getinfo(flash);
    TEST_ASSERT_NOT_EQUAL(NULL, info);
    TEST_ASSERT_NOT_EQUAL(0, info->pageSize);
    TEST_ASSERT_GREATER_OR_EQUAL(info->pageSize,
                                 info->sectorSize);
    TEST_ASSERT_GREATER_OR_EQUAL(info->sectorSize,
                                 info->blockSize);
    TEST_ASSERT_GREATER_OR_EQUAL(info->blockSize,
                                 info->flashSize);
    TEST_ASSERT_EQUAL(0,
                      info->pageSize & (info->pageSize - 1));
    TEST_ASSERT_EQUAL(0, info->sectorSize &
                             (info->sectorSize - 1));
    TEST_ASSERT_EQUAL(0, info->blockSize &
                             (info->blockSize - 1));
    TEST_ASSERT_EQUAL(0, info->flashSize &
                             (info->flashSize - 1));
  }
  result = flash_close(flash);
  TEST_ASSERT_EQUAL(FlashSuccess, result);
}

TEST(TestFlash, FlashEraseSector) {
  Flash flash;
  FlashInfo *info;
  int result;

  flash = flash_open(Instance_);
  TEST_ASSERT_NOT_EQUAL(NULL, flash);

  if (TEST_PROTECT()) {
    info = flash_getinfo(flash);
    TEST_ASSERT_NOT_EQUAL(NULL, info);		
    result =
        Flash_eraseSectors(flash, startOffset_, info->sectorSize);
    TEST_ASSERT_EQUAL(FlashSuccess, result);

    writeReadVerify_(flash, startOffset_, info->sectorSize);

    result =
        flash_erase_sectors(flash, startOffset_, info->sectorSize);
    TEST_ASSERT_EQUAL(FlashSuccess, result);

    readVerifyErased_(flash, startOffset_, info->sectorSize);
  }
  result = flash_close(flash);
  TEST_ASSERT_EQUAL(FlashSuccess, result);
}

TEST(TestFlash, FlashEraseMultipleSectors) {
  Flash flash;
  FlashInfo *info;
  int result;

  flash = flash_open(Instance_);
  TEST_ASSERT_NOT_EQUAL(NULL, flash);

  if (TEST_PROTECT()) {
    info = flash_getinfo(flash);
    TEST_ASSERT_NOT_EQUAL(NULL, info);

    result =
        flash_erase_sectors(flash, startOffset_, info->sectorSize * 2);
    TEST_ASSERT_EQUAL(FlashSuccess, result);

    if (info->ucAsyncSupported) {
      /* Wait for the Erase to be completed and callback is called. */
      result = xWaitCounterTake(xFlashWaitCounter, portMAX_DELAY);
      TEST_ASSERT_EQUAL(pdTRUE, result);
    }

    /* Read back the two sectors to make sure the contents are erased.
     */
    readVerifyErased_(flash, startOffset_, info->sectorSize * 2);
  }

  /* Close flash handle */
  result = flash_close(flash);
  TEST_ASSERT_EQUAL(FlashSuccess, result);
}

/**
 * @brief Test Function to test erasing various size blocks (32K, 64K)
 * and make sure erase is being done correctly.
 *
 */
TEST(TestFlash, FlashEraseFlashBlocks) {
  Flash flash;
  FlashInfo *info;
  int result;
  int ulMiddleOffset;

  /* Open the flash Instance_ */
  flash = flash_open(Instance_);
  TEST_ASSERT_NOT_EQUAL(NULL, flash);

  if (TEST_PROTECT()) {
    /* Get the flash information. */
    info = flash_getinfo(flash);
    TEST_ASSERT_NOT_EQUAL(NULL, info);

    /* Make sure the offset is aligned to BlockSize */
    if ((startOffset_ & (info->ulBlockSize - 1)) == 0) {
      /* If Erase asyc is supported, register a callback */
      if (info->ucAsyncSupported) {
        flash_set_callback(flash, prvFlashEraseCallback, NULL);
      }

      /* Erase the 2 sectors before writing to it. */
      result = flash_erase_sectors(flash, startOffset_,
                                   info->sectorSize * 2);
      TEST_ASSERT_EQUAL(FlashSuccess, result);

      if (info->ucAsyncSupported) {
        /* Wait for the Erase to be completed and callback is called. */
        result = xWaitCounterTake(xFlashWaitCounter, portMAX_DELAY);
        TEST_ASSERT_EQUAL(pdTRUE, result);
      }

      ulMiddleOffset = (startOffset_ + (info->sectorSize / 2));

      /*
       * Write dummy data starting at the middle of the sector to half of next
       * sector and read it back to verify the data.
       */
      writeReadVerify_(flash, ulMiddleOffset, info->sectorSize);

      /* Erase a Block */
      result =
          flash_erase_sectors(flash, startOffset_, info->ulBlockSize);
      TEST_ASSERT_EQUAL(FlashSuccess, result);

      if (info->ucAsyncSupported) {
        /* Wait for the Erase to be completed and callback is called. */
        result = xWaitCounterTake(xFlashWaitCounter, portMAX_DELAY);
        TEST_ASSERT_EQUAL(pdTRUE, result);
      }

      /* Read back the block to make sure the contents are erased.
       */
      readVerifyErased_(flash, startOffset_, info->ulBlockSize);
    } else {
      /* TEST_MESSAGE( "Start offset is not aligned with blockSize" ); */
    }
  }

  /* Close flash handle */
  result = flash_close(flash);
  TEST_ASSERT_EQUAL(FlashSuccess, result);
}

/**
 * @brief Test Function to test erasing blockSize minus sector Size when address
 * is one sector greater than block alignemtnt, and make sure the sector not
 * included in the Address is not erased, but the rest is erased.
 *
 */
TEST(TestFlash, FlashEraseFlashBlocksUnAlignedAddress) {
  Flash flash;
  FlashInfo *info;
  int result;
  int ulMiddleOffset;

  /* Open the flash Instance_ */
  flash = flash_open(Instance_);
  TEST_ASSERT_NOT_EQUAL(NULL, flash);

  if (TEST_PROTECT()) {
    /* Get the flash information. */
    info = flash_getinfo(flash);
    TEST_ASSERT_NOT_EQUAL(NULL, info);

    /* Make sure the offset is aligned to BlockSize */
    if ((startOffset_ & (info->ulBlockSize - 1)) == 0) {
      /* If Erase asyc is supported, register a callback */
      if (info->ucAsyncSupported) {
        flash_set_callback(flash, prvFlashEraseCallback, NULL);
      }

      /* Erase the 2 sectors before writing to it. */
      result = flash_erase_sectors(flash, startOffset_,
                                   info->sectorSize * 2);
      TEST_ASSERT_EQUAL(FlashSuccess, result);

      if (info->ucAsyncSupported) {
        /* Wait for the Erase to be completed and callback is called. */
        result = xWaitCounterTake(xFlashWaitCounter, portMAX_DELAY);
        TEST_ASSERT_EQUAL(pdTRUE, result);
      }

      ulMiddleOffset = (startOffset_ + (info->sectorSize / 2));

      /*
       * Write dummy data starting at the middle of the sector to half of next
       * sector and read it back to verify the data.
       */
      writeReadVerify_(flash, ulMiddleOffset, info->sectorSize);

      /* Erase a Block minus sector size starting at block boundry plus sector
       * size */
      result = flash_erase_sectors(
          flash, startOffset_ + info->sectorSize,
          info->ulBlockSize - info->sectorSize);
      TEST_ASSERT_EQUAL(FlashSuccess, result);

      if (info->ucAsyncSupported) {
        /* Wait for the Erase to be completed and callback is called. */
        result = xWaitCounterTake(xFlashWaitCounter, portMAX_DELAY);
        TEST_ASSERT_EQUAL(pdTRUE, result);
      }

      /* Read back the block to make sure the contents are erased.
       */
      readVerifyErased_(flash, startOffset_ + info->sectorSize,
                        info->ulBlockSize - info->sectorSize);

      /* Read and make sure that the first sector written is not erased */
      ulMiddleOffset = (startOffset_ + (info->sectorSize / 2));

      readVerifyDummyData_(flash, ulMiddleOffset,
                           info->sectorSize / 2);
    } else {
      /* TEST_MESSAGE( "Start offset is not aligned with blockSize" ); */
    }
  }

  /* Close flash handle */
  result = flash_close(flash);
  TEST_ASSERT_EQUAL(FlashSuccess, result);
}

/**
 * @brief Test Function to test erasing blockSize minus sector Size when size is
 * one sector less than block alignemtnt, and make sure the sector not included
 * in the block size is not erased, but the rest is erased.
 *
 */
TEST(TestFlash, FlashEraseFlashBlocksUnAlignedSize) {
  Flash flash;
  FlashInfo *info;
  int result;
  int offset;

  /* Open the flash Instance_ */
  flash = flash_open(Instance_);
  TEST_ASSERT_NOT_EQUAL(NULL, flash);

  if (TEST_PROTECT()) {
    /* Get the flash information. */
    info = flash_getinfo(flash);
    TEST_ASSERT_NOT_EQUAL(NULL, info);

    /* Make sure the offset is aligned to BlockSize */
    if ((startOffset_ & (info->ulBlockSize - 1)) == 0) {
      /* If Erase asyc is supported, register a callback */
      if (info->ucAsyncSupported) {
        flash_set_callback(flash, prvFlashEraseCallback, NULL);
      }

      /* Erase the 2 sectors before writing to it. */
      result = flash_erase_sectors(flash, startOffset_,
                                   info->sectorSize * 2);
      TEST_ASSERT_EQUAL(FlashSuccess, result);

      if (info->ucAsyncSupported) {
        /* Wait for the Erase to be completed and callback is called. */
        result = xWaitCounterTake(xFlashWaitCounter, portMAX_DELAY);
        TEST_ASSERT_EQUAL(pdTRUE, result);
      }

      offset = (startOffset_ + (info->sectorSize / 2));

      /*
       * Write dummy data starting at the middle of the sector to half of next
       * sector and read it back to verify the data.
       */
      writeReadVerify_(flash, offset, info->sectorSize);

      /* Also write the last sector in the block to make sure its not erased */
      result = flash_erase_sectors(
          flash,
          startOffset_ + (info->ulBlockSize - info->sectorSize),
          info->sectorSize);
      TEST_ASSERT_EQUAL(FlashSuccess, result);

      offset = (startOffset_ +
                  (info->ulBlockSize - info->sectorSize));

      writeReadVerify_(flash, offset, info->sectorSize);

      /* Erase a Block minus sector size starting at block boundry */
      result = flash_erase_sectors(flash, startOffset_,
                                   info->ulBlockSize -
                                       info->sectorSize);
      TEST_ASSERT_EQUAL(FlashSuccess, result);

      if (info->ucAsyncSupported) {
        /* Wait for the Erase to be completed and callback is called. */
        result = xWaitCounterTake(xFlashWaitCounter, portMAX_DELAY);
        TEST_ASSERT_EQUAL(pdTRUE, result);
      }

      /* Read back the block to make sure the contents are erased.
       */
      readVerifyErased_(flash, startOffset_,
                        info->ulBlockSize - info->sectorSize);

      /* Read and make sure that the last sector written is not erased */
      offset = (startOffset_ +
                  (info->ulBlockSize - info->sectorSize));
      readVerifyDummyData_(flash, offset, info->sectorSize);
    } else {
      /* TEST_MESSAGE( "Start offset is not aligned with blockSize" ); */
    }
  }

  /* Close flash handle */
  result = flash_close(flash);
  TEST_ASSERT_EQUAL(FlashSuccess, result);
}

/**
 * @brief Test Function to test erase return an error when un-aligned address is
 * passed to erase function
 *
 */
TEST(TestFlash, FlashEraseUnAlignedAddress) {
  Flash flash;
  FlashInfo *info;
  int result;

  /* Open the flash Instance_ */
  flash = flash_open(Instance_);
  TEST_ASSERT_NOT_EQUAL(NULL, flash);

  if (TEST_PROTECT()) {
    /* Get the flash information. */
    info = flash_getinfo(flash);
    TEST_ASSERT_NOT_EQUAL(NULL, info);

    /* Try erasing sector where offset is not aligned. */
    result =
        flash_erase_sectors(flash, startOffset_ + 1, info->sectorSize);
    TEST_ASSERT_EQUAL(FLASH_INVALID_VALUE, result);
  }

  /* Close flash handle */
  result = flash_close(flash);
  TEST_ASSERT_EQUAL(FlashSuccess, result);
}

/**
 * @brief Test Function to test erase return an error when un-aligned size is
 * passed to erase function
 *
 */
TEST(TestFlash, FlashEraseUnAlignedSize) {
  Flash flash;
  FlashInfo *info;
  int result;

  /* Open the flash Instance_ */
  flash = flash_open(Instance_);
  TEST_ASSERT_NOT_EQUAL(NULL, flash);

  if (TEST_PROTECT()) {
    /* Get the flash information. */
    info = flash_getinfo(flash);
    TEST_ASSERT_NOT_EQUAL(NULL, info);

    /* Try erasing sector where offset is not aligned. */
    result =
        flash_erase_sectors(flash, startOffset_, info->sectorSize + 1);
    TEST_ASSERT_EQUAL(FLASH_INVALID_VALUE, result);
  }

  /* Close flash handle */
  result = flash_close(flash);
  TEST_ASSERT_EQUAL(FlashSuccess, result);
}

/**
 * @brief Test Function to write to a page with multiple writes,
 * and make sure that the read contents are correct.
 *
 */
TEST(TestFlash, FlashWritePartialPage) {
  Flash flash;
  FlashInfo *info;
  int result;
  int8_t i;

  /* Open the flash Instance_ */
  flash = flash_open(Instance_);
  TEST_ASSERT_NOT_EQUAL(NULL, flash);

  if (TEST_PROTECT()) {
    /* Get the flash information. */
    info = flash_getinfo(flash);
    TEST_ASSERT_NOT_EQUAL(NULL, info);

    /* If Erase asyc is supported, register a callback */
    if (info->ucAsyncSupported) {
      flash_set_callback(flash, prvFlashEraseCallback, NULL);
    }

    /* Erase the sector before writing to it. */
    result =
        flash_erase_sectors(flash, startOffset_, info->sectorSize);
    TEST_ASSERT_EQUAL(FlashSuccess, result);

    if (info->ucAsyncSupported) {
      /* Wait for the Erase to be completed and callback is called. */
      result = xWaitCounterTake(xFlashWaitCounter, portMAX_DELAY);
      TEST_ASSERT_EQUAL(pdTRUE, result);
    }

    /* fill out a buffer (Partial page) to be written */
    for (i = 0; i < FLASH_DEFAULT_PARTIAL_PAGE_SIZE * 2; i++) {
      writeBuffer_[i] = i;
    }

    /* Write partial page (< page size) */
    result = Flash_writeSync(flash, startOffset_, &writeBuffer_[0],
                             FLASH_DEFAULT_PARTIAL_PAGE_SIZE);
    TEST_ASSERT_EQUAL(FlashSuccess, result);

    /* Write partial page starting from the end of previous
     * write and make sure the second write does not overwrite the first
     */
    result =
        Flash_writeSync(flash, startOffset_ + FLASH_DEFAULT_PARTIAL_PAGE_SIZE,
                        &writeBuffer_[FLASH_DEFAULT_PARTIAL_PAGE_SIZE],
                        FLASH_DEFAULT_PARTIAL_PAGE_SIZE);
    TEST_ASSERT_EQUAL(FlashSuccess, result);

    /* Read back the data written */
    result = Flash_readSync(flash, startOffset_, &readBuffer_[0],
                            FLASH_DEFAULT_PARTIAL_PAGE_SIZE * 2);
    TEST_ASSERT_EQUAL(FlashSuccess, result);

    /* Verify the data written. */
    for (i = 0; i < FLASH_DEFAULT_PARTIAL_PAGE_SIZE * 2; i++) {
      if (readBuffer_[i] != writeBuffer_[i]) {
        TEST_ASSERT_MESSAGE(0, "Contents do NOT match when readback");
      }
    }
  }

  /* Close flash handle */
  result = flash_close(flash);
  TEST_ASSERT_EQUAL(FlashSuccess, result);
}

/**
 * @brief Test Function to test writing a page to flash and verify contents.
 *
 */
TEST(TestFlash, FlashWritePage) {
  Flash flash;
  FlashInfo *info;
  int result;

  /* Open the flash Instance_ */
  flash = flash_open(Instance_);
  TEST_ASSERT_NOT_EQUAL(NULL, flash);

  if (TEST_PROTECT()) {
    /* Get the flash information. */
    info = flash_getinfo(flash);
    TEST_ASSERT_NOT_EQUAL(NULL, info);

    /* If Erase asyc is supported, register a callback */
    if (info->ucAsyncSupported) {
      flash_set_callback(flash, prvFlashEraseCallback, NULL);
    }

    /* Erase the sector before writing to it. */
    result =
        flash_erase_sectors(flash, startOffset_, info->sectorSize);
    TEST_ASSERT_EQUAL(FlashSuccess, result);

    if (info->ucAsyncSupported) {
      /* Wait for the Erase to be completed and callback is called. */
      result = xWaitCounterTake(xFlashWaitCounter, portMAX_DELAY);
      TEST_ASSERT_EQUAL(pdTRUE, result);
    }

    /* Write the a full page and verify the data by reading it back */
    writeReadVerify_(flash, startOffset_, info->ulPageSize);
  }

  /* Close flash handle */
  result = flash_close(flash);
  TEST_ASSERT_EQUAL(FlashSuccess, result);
}

/**
 * @brief Test Function to test writing greater than page but less than or equal
 * to sector to flash and verify contents.
 *
 */
TEST(TestFlash, FlashWriteSector) {
  Flash flash;
  FlashInfo *info;
  int result;

  /* Open the flash Instance_ */
  flash = flash_open(Instance_);
  TEST_ASSERT_NOT_EQUAL(NULL, flash);

  if (TEST_PROTECT()) {
    /* Get the flash information. */
    info = flash_getinfo(flash);
    TEST_ASSERT_NOT_EQUAL(NULL, info);

    /* If Erase asyc is supported, register a callback */
    if (info->ucAsyncSupported) {
      flash_set_callback(flash, prvFlashEraseCallback, NULL);
    }

    /* Erase the sector before writing to it. */
    result =
        flash_erase_sectors(flash, startOffset_, info->sectorSize);
    TEST_ASSERT_EQUAL(FlashSuccess, result);

    if (info->ucAsyncSupported) {
      /* Wait for the Erase to be completed and callback is called. */
      result = xWaitCounterTake(xFlashWaitCounter, portMAX_DELAY);
      TEST_ASSERT_EQUAL(pdTRUE, result);
    }

    /* Write the a full sector and verify the data by reading it back */
    writeReadVerify_(flash, startOffset_, info->sectorSize);
  }

  /* Close flash handle */
  result = flash_close(flash);
  TEST_ASSERT_EQUAL(FlashSuccess, result);
}

TEST(TestFlash, FlashWriteAcrossSectors) {
  Flash flash;
  FlashInfo *info;
  int result;
  int offset;

  flash = flash_open(Instance_);
  TEST_ASSERT_NOT_EQUAL(NULL, flash);

  if (TEST_PROTECT()) {
    info = Flash_getInfo(flash);
    TEST_ASSERT_NOT_EQUAL(NULL, info);

    result =
        flash_erase_sectors(flash, startOffset_, info->sectorSize * 2);
    TEST_ASSERT_EQUAL(FlashSuccess, result);

    offset = (startOffset_ + (info->sectorSize / 2));

    writeReadVerify_(flash, offset, info->sectorSize);
  }
  result = flash_close(flash);
  TEST_ASSERT_EQUAL(FlashSuccess, result);
}

TEST(TestFlash, FlashEraseInProgressRead) {
  Flash flash;
  FlashInfo *info;
  int result;
  int size;

  flash = flash_open(Instance_);
  TEST_ASSERT_NOT_EQUAL(NULL, flash);

  if (TEST_PROTECT()) {
    /* Get the flash information. */
    info = flash_getinfo(flash);
    TEST_ASSERT_NOT_EQUAL(NULL, info);

    /* Erase multiple sectors to get some time for read while erase. */
    size = info->sectorSize * 2;
    result = flash_erase_sectors(flash, startOffset_, size);
    TEST_ASSERT_EQUAL(FlashSuccess, result);

    /* Try reading the data from the same sector(s) being erased. */
    result = Flash_readSync(flash, startOffset_, &readBuffer_[0],
                            FLASH_DEFAULT_PARTIAL_PAGE_SIZE);

    if (result != FLASH_DEVICE_BUSY) {
      TEST_ASSERT_EQUAL(FlashSuccess, result);
    }
  }

  /* Close flash handle */
  result = flash_close(flash);
  TEST_ASSERT_EQUAL(FlashSuccess, result);
}

/**
 * @brief Test Function to test Write when Flash Erase is in progress.
 *
 */
TEST(TestFlash, FlashEraseInProgressWrite) {
  Flash flash;
  FlashInfo *info;
  int result;
  int size;
  int8_t i;

  /* Open the flash Instance_ */
  flash = flash_open(Instance_);
  TEST_ASSERT_NOT_EQUAL(NULL, flash);

  if (TEST_PROTECT()) {
    /* Get the flash information. */
    info = flash_getinfo(flash);
    TEST_ASSERT_NOT_EQUAL(NULL, info);

    /* Erase multiple sectors to get some time for read while erase. */
    size = info->sectorSize * 2;
    result = flash_erase_sectors(flash, startOffset_, size);
    TEST_ASSERT_EQUAL(FlashSuccess, result);

    /* Fill out a buffer of sectorSize to be written */
    for (i = 0; i < FLASH_DEFAULT_PARTIAL_PAGE_SIZE; i++) {
      writeBuffer_[i] = i;
    }

    /* Try writing the the same sector when erase is in progress. */
    result = Flash_writeSync(flash, startOffset_, &writeBuffer_[0],
                             FLASH_DEFAULT_PARTIAL_PAGE_SIZE);

    if (result != FLASH_DEVICE_BUSY) {
      TEST_ASSERT_EQUAL(FlashSuccess, result);
      /* Read back the data written */
      result = Flash_readSync(flash, startOffset_, &readBuffer_[0],
                              FLASH_DEFAULT_PARTIAL_PAGE_SIZE);
      TEST_ASSERT_EQUAL(FlashSuccess, result);

      /* Verify the data written. */
      for (i = 0; i < FLASH_DEFAULT_PARTIAL_PAGE_SIZE; i++) {
        if (readBuffer_[i] != writeBuffer_[i]) {
          TEST_ASSERT_MESSAGE(0, "Contents do NOT match when readback");
        }
      }
    }
  }

  /* Close flash handle */
  result = flash_close(flash);
  TEST_ASSERT_EQUAL(FlashSuccess, result);
}

/**
 * @brief Test Function to test Write failure when write protection is enabled.
 *
 */
TEST(TestFlash, FlashWriteProtectWriteFailure) {
  Flash flash;
  FlashInfo *info;
  int result;
  FlashWriteProtectConfig_t xFlashProtect;

  /* Open the flash Instance_ */
  flash = flash_open(Instance_);
  TEST_ASSERT_NOT_EQUAL(NULL, flash);

  if (TEST_PROTECT()) {
    /* Get the flash information. */
    info = flash_getinfo(flash);
    TEST_ASSERT_NOT_EQUAL(NULL, info);

    /* If Erase asyc is supported, register a callback */
    if (info->ucAsyncSupported) {
      flash_set_callback(flash, prvFlashEraseCallback, NULL);
    }

    /* Erase a sector before trying to write to it. */
    result =
        flash_erase_sectors(flash, startOffset_, info->sectorSize);
    TEST_ASSERT_EQUAL(FlashSuccess, result);

    if (info->ucAsyncSupported) {
      /* Wait for the Erase to be completed and callback is called. */
      result = xWaitCounterTake(xFlashWaitCounter, portMAX_DELAY);
      TEST_ASSERT_EQUAL(pdTRUE, result);
    }

    /* Make the sector as read only. */
    xFlashProtect.ulAddress = startOffset_;
    xFlashProtect.size = info->ulLockSupportSize;
    xFlashProtect.xProtectionLevel = eFlashReadOnly;

    result = flash_ioctl(flash, eSetFlashBlockProtection,
                         (void *const) & xFlashProtect);

    /* Check if block protection is supported */
    if (result != FLASH_FUNCTION_NOT_SUPPORTED) {
      TEST_ASSERT_EQUAL(FlashSuccess, result);

      /* Read back the block protection */
      result = flash_ioctl(flash, eGetFlashBlockProtection,
                           (void *const) & xFlashProtect);

      TEST_ASSERT_EQUAL(FlashSuccess, result);

      /* Write the a full page and verify the data is not written reading it
       * back */
      writeDummyData_(flash, startOffset_, info->ulPageSize);
      readVerifyErased_(flash, startOffset_, info->ulPageSize);
    }

    /* Now make the sector writeable */
    xFlashProtect.ulAddress = startOffset_;
    xFlashProtect.size = info->ulLockSupportSize;
    xFlashProtect.xProtectionLevel = eFlashReadWrite;
    result = flash_ioctl(flash, eSetFlashBlockProtection,
                         (void *const) & xFlashProtect);

    /* Check if block protection is supported */
    if (result != FLASH_FUNCTION_NOT_SUPPORTED) {
      TEST_ASSERT_EQUAL(FlashSuccess, result);

      /* Read back the block protection */
      result = flash_ioctl(flash, eGetFlashBlockProtection,
                           (void *const) & xFlashProtect);
      TEST_ASSERT_EQUAL(FlashSuccess, result);

      /* Write the a full page and verify the data by reading it back */
      writeReadVerify_(flash, startOffset_, info->ulPageSize);
    }
  }

  /* Close flash handle */
  result = flash_close(flash);
  TEST_ASSERT_EQUAL(FlashSuccess, result);
}

/**
 * @brief Test Function to test Erase failure when write protect is enabled.
 *
 */
TEST(TestFlash, FlashWriteProtectEraseFailure) {
  Flash flash;
  FlashInfo *info;
  int result;
  FlashWriteProtectConfig_t xFlashProtect;

  /* Open the flash Instance_ */
  flash = flash_open(Instance_);
  TEST_ASSERT_NOT_EQUAL(NULL, flash);

  if (TEST_PROTECT()) {
    /* Get the flash information. */
    info = flash_getinfo(flash);
    TEST_ASSERT_NOT_EQUAL(NULL, info);

    /* If Erase asyc is supported, register a callback */
    if (info->ucAsyncSupported) {
      flash_set_callback(flash, prvFlashEraseCallback, NULL);
    }

    /* Erase a sector before trying to write to it. */
    result =
        flash_erase_sectors(flash, startOffset_, info->sectorSize);
    TEST_ASSERT_EQUAL(FlashSuccess, result);

    if (info->ucAsyncSupported) {
      /* Wait for the Erase to be completed and callback is called. */
      result = xWaitCounterTake(xFlashWaitCounter, portMAX_DELAY);
      TEST_ASSERT_EQUAL(pdTRUE, result);
    }

    /* Verify that the page was erased */
    readVerifyErased_(flash, startOffset_, info->ulPageSize);

    /* Fill out a buffer of sectorSize to be written */
    writeDummyData_(flash, startOffset_, info->ulPageSize);

    /* Make the sector as read only. */
    xFlashProtect.ulAddress = startOffset_;
    xFlashProtect.size = info->ulLockSupportSize;
    xFlashProtect.xProtectionLevel = eFlashReadOnly;
    result = flash_ioctl(flash, eSetFlashBlockProtection,
                         (void *const) & xFlashProtect);

    /* Check if block protection is supported */
    if (result != FLASH_FUNCTION_NOT_SUPPORTED) {
      TEST_ASSERT_EQUAL(FlashSuccess, result);

      /* Read back the block protection */
      result = flash_ioctl(flash, eGetFlashBlockProtection,
                           (void *const) & xFlashProtect);
      TEST_ASSERT_EQUAL(FlashSuccess, result);

      /* Try erasing the sector. */
      result =
          flash_erase_sectors(flash, startOffset_, info->sectorSize);
      TEST_ASSERT_EQUAL(FlashSuccess, result);

      if (info->ucAsyncSupported) {
        /* Wait for the Erase to be completed and callback is called. */
        result = xWaitCounterTake(xFlashWaitCounter, portMAX_DELAY);
        TEST_ASSERT_EQUAL(pdTRUE, result);
      }

      /* Read back the data written */
      readVerifyDummyData_(flash, startOffset_, info->ulPageSize);
    }

    /* Now make the sector as writeable. */
    xFlashProtect.ulAddress = startOffset_;
    xFlashProtect.size = info->ulLockSupportSize;
    xFlashProtect.xProtectionLevel = eFlashReadWrite;
    result = flash_ioctl(flash, eSetFlashBlockProtection,
                         (void *const) & xFlashProtect);

    /* Check if block protection is supported */
    if (result != FLASH_FUNCTION_NOT_SUPPORTED) {
      TEST_ASSERT_EQUAL(FlashSuccess, result);

      /* Read back the block protection */
      result = flash_ioctl(flash, eGetFlashBlockProtection,
                           (void *const) & xFlashProtect);
      TEST_ASSERT_EQUAL(FlashSuccess, result);

      /* Try erasing the sector. */
      result =
          flash_erase_sectors(flash, startOffset_, info->sectorSize);
      TEST_ASSERT_EQUAL(FlashSuccess, result);

      if (info->ucAsyncSupported) {
        /* Wait for the Erase to be completed and callback is called. */
        result = xWaitCounterTake(xFlashWaitCounter, portMAX_DELAY);
        TEST_ASSERT_EQUAL(pdTRUE, result);
      }

      /* Read back the data written and verify that it is erased */
      readVerifyErased_(flash, startOffset_, info->ulPageSize);
    }
  }

  /* Close flash handle */
  result = flash_close(flash);
  TEST_ASSERT_EQUAL(FlashSuccess, result);
}

/**
 * @brief Test Function to test Erase-Read-Write-Read-Erase-Read cycle
 *
 */
TEST(TestFlash, FlashWriteEraseReadCycle) {
  Flash flash;
  FlashInfo *info;
  int result;
  int idx;

  /* Open the flash Instance_ */
  flash = flash_open(Instance_);
  TEST_ASSERT_NOT_EQUAL(NULL, flash);

  if (TEST_PROTECT()) {
    /* Get the flash information. */
    info = flash_getinfo(flash);
    TEST_ASSERT_NOT_EQUAL(NULL, info);

    /* Erase a sector before trying to write to it. */
    result =
        flash_erase_sectors(flash, startOffset_, info->sectorSize);
    TEST_ASSERT_EQUAL(FlashSuccess, result);

    /* Read back the data and make sure that data is erased first */
    readVerifyErased_(flash, startOffset_, info->ulPageSize);

    for (idx = 0; idx < 5; idx++) {
      /* Write to this sector and veirfy */
      writeReadVerify_(flash, startOffset_, info->ulPageSize);

      /* Get the flash information. */
      info = flash_getinfo(flash);
      TEST_ASSERT_NOT_EQUAL(NULL, info);

      /* Erase the sector. */
      result =
          flash_erase_sectors(flash, startOffset_, info->sectorSize);
      TEST_ASSERT_EQUAL(FlashSuccess, result);

      /* Read back the data and make sure that data is erased */
      readVerifyErased_(flash, startOffset_, info->ulPageSize);
    }
  }

  /* Close flash handle */
  result = flash_close(flash);
  TEST_ASSERT_EQUAL(FlashSuccess, result);
}

/**
 * @brief Test Function to test Write susepnd and resume operations
 *
 */
TEST(TestFlash, FlashWriteSuspendResume) {
  Flash flash;
  FlashInfo *info;
  Timer xTimerHandle;
  int result;
  int lStatus;

  /* Open the flash Instance_ */
  flash = flash_open(Instance_);
  TEST_ASSERT_NOT_EQUAL(NULL, flash);

  if (TEST_PROTECT()) {
    /* Setup a timer to suspend the write */
    xTimerHandle = timer_open(lTimerInstance_);
    TEST_ASSERT_NOT_EQUAL(NULL, xTimerHandle);

    /* Get the flash information. */
    info = flash_getinfo(flash);
    TEST_ASSERT_NOT_EQUAL(NULL, info);

    /* If Erase asyc is supported, register a callback */
    if (info->ucAsyncSupported) {
      flash_set_callback(flash, prvFlashEraseCallback, NULL);
    }

    /* Erase the 2 sectors before writing to it. */
    result =
        flash_erase_sectors(flash, startOffset_, info->sectorSize * 2);
    TEST_ASSERT_EQUAL(FlashSuccess, result);

    if (info->ucAsyncSupported) {
      /* Wait for the Erase to be completed and callback is called. */
      result = xWaitCounterTake(xFlashWaitCounter, portMAX_DELAY);
      TEST_ASSERT_EQUAL(pdTRUE, result);
    }

    /* Set up the timer callback */
    timer_set_callback(xTimerHandle, prvTimerCallback, flash);

    /* Set up the timer delay */
    result = timer_delay(xTimerHandle, FLASH_DEFAULT_DELAY_US);
    TEST_ASSERT_EQUAL(TIMER_SUCCESS, result);

    /* Start the timer */
    result = timer_start(xTimerHandle);
    TEST_ASSERT_EQUAL(TIMER_SUCCESS, result);

    /* Fill out a sector with data */
    writeDummyData_(flash, startOffset_, info->sectorSize);

    /* Wait for the Delay callback to be called. */
    result = xWaitCounterTake(xFlashTimerWaitCounter, portMAX_DELAY);
    TEST_ASSERT_EQUAL(pdTRUE, result);

    /* Verify that current write is suspended */
    result = flash_ioctl(flash, eGetFlashStatus, (void *const) & lStatus);
    TEST_ASSERT_EQUAL(FlashSuccess, result);
    TEST_ASSERT_EQUAL(eFlashProgramSuspended, lStatus);

    /* Resume the current write operation */
    /* Suspend the flash write/erase */
    result = flash_ioctl(flash, eResumeFlashProgramErase, NULL);

    /* Check if suspend/resume is supported */
    if (result != FLASH_FUNCTION_NOT_SUPPORTED) {
      TEST_ASSERT_EQUAL(FlashSuccess, result);
    }

    /* Verify that current write is resumed */
    result = flash_ioctl(flash, eGetFlashStatus, (void *const) & lStatus);
    TEST_ASSERT_EQUAL(FlashSuccess, result);
    TEST_ASSERT_EQUAL(eFlashIdle, lStatus);

    /* Read the data back to make sure Resume succeded and data is written */
    readVerifyDummyData_(flash, startOffset_, info->sectorSize);

    result = timer_close(xTimerHandle);
    TEST_ASSERT_EQUAL(TIMER_SUCCESS, result);
  }

  /* Close flash handle */
  result = flash_close(flash);
  TEST_ASSERT_EQUAL(FlashSuccess, result);
}

/**
 * @brief Test Function to test Erase susepnd and resume operations
 *
 */
TEST(TestFlash, FlashEraseSuspendResume) {
  Flash flash;
  FlashInfo *info;
  Timer xTimerHandle;
  int result;
  int lStatus;

  /* Open the flash Instance_ */
  flash = flash_open(Instance_);
  TEST_ASSERT_NOT_EQUAL(NULL, flash);

  if (TEST_PROTECT()) {
    /* Setup a timer to suspend the write */
    xTimerHandle = timer_open(lTimerInstance_);
    TEST_ASSERT_NOT_EQUAL(NULL, xTimerHandle);

    /* Get the flash information. */
    info = flash_getinfo(flash);
    TEST_ASSERT_NOT_EQUAL(NULL, info);

    /* Set up the timer callback */
    timer_set_callback(xTimerHandle, prvTimerCallback, flash);

    /* Set up the timer delay */
    result = timer_delay(xTimerHandle, FLASH_DEFAULT_DELAY_US);
    TEST_ASSERT_EQUAL(TIMER_SUCCESS, result);

    /* Start the timer */
    result = timer_start(xTimerHandle);
    TEST_ASSERT_EQUAL(TIMER_SUCCESS, result);

    /* If Erase asyc is supported, register a callback */
    if (info->ucAsyncSupported) {
      flash_set_callback(flash, prvFlashEraseCallback, NULL);
    }

    /* Erase the 2 sectors before writing to it. */
    result =
        flash_erase_sectors(flash, startOffset_, info->sectorSize * 2);
    TEST_ASSERT_EQUAL(FlashSuccess, result);

    /* Wait for the Delay callback to be called. */
    result = xWaitCounterTake(xFlashTimerWaitCounter, portMAX_DELAY);
    TEST_ASSERT_EQUAL(pdTRUE, result);

    /* Verify that current erase is suspended */
    result = flash_ioctl(flash, eGetFlashStatus, (void *const) & lStatus);
    TEST_ASSERT_EQUAL(FlashSuccess, result);
    TEST_ASSERT_EQUAL(eFlashEraseSuspended, lStatus);

    /* Resume the current erase operation */
    result = flash_ioctl(flash, eResumeFlashProgramErase, NULL);

    /* Check if suspend/resume is supported */
    if (result != FLASH_FUNCTION_NOT_SUPPORTED) {
      TEST_ASSERT_EQUAL(FlashSuccess, result);
    }

    /* Verify that current erase is resumed */
    result = flash_ioctl(flash, eGetFlashStatus, (void *const) & lStatus);
    TEST_ASSERT_EQUAL(FlashSuccess, result);
    TEST_ASSERT_EQUAL(eFlashIdle, lStatus);

    if (info->ucAsyncSupported) {
      /* Wait for the Erase to be completed and callback is called. */
      result = xWaitCounterTake(xFlashWaitCounter, portMAX_DELAY);
      TEST_ASSERT_EQUAL(pdTRUE, result);
    }

    /* Read the data back to make sure Resume succeded and data is erased */
    readVerifyErased_(flash, startOffset_, info->sectorSize * 2);

    result = timer_close(xTimerHandle);
    TEST_ASSERT_EQUAL(TIMER_SUCCESS, result);
  }

  /* Close flash handle */
  result = flash_close(flash);
  TEST_ASSERT_EQUAL(FlashSuccess, result);
}

/**
 * @brief Test Function to test writing a page to flash and verify contents.
 *
 */
TEST(TestFlash, FlashWritePageFromFlash) {
  Flash flash;
  FlashInfo *info;
  int result;
  int i;
  int j;

  /* Open the flash Instance_ */
  flash = flash_open(Instance_);
  TEST_ASSERT_NOT_EQUAL(NULL, flash);

  if (TEST_PROTECT()) {
    /* Get the flash information. */
    info = flash_getinfo(flash);
    TEST_ASSERT_NOT_EQUAL(NULL, info);

    /* If Erase asyc is supported, register a callback */
    if (info->ucAsyncSupported) {
      flash_set_callback(flash, prvFlashEraseCallback, NULL);
    }

    /* Erase the sector before writing to it. */
    result =
        flash_erase_sectors(flash, startOffset_, info->sectorSize);
    TEST_ASSERT_EQUAL(FlashSuccess, result);

    if (info->ucAsyncSupported) {
      /* Wait for the Erase to be completed and callback is called. */
      result = xWaitCounterTake(xFlashWaitCounter, portMAX_DELAY);
      TEST_ASSERT_EQUAL(pdTRUE, result);
    }

    /* Write full page of data */
    for (i = 0; i < info->ulPageSize; i += FlashDefaultMaxBufferSize) {
      /* Less the a full buffer left? */
      int remainingSize =
          ((i + FlashDefaultMaxBufferSize) > info->ulPageSize)
              ? (info->ulPageSize - i)
              : FlashDefaultMaxBufferSize;

      if (remainingSize > 0) {
        for (j = 0; j < remainingSize; j++) {
          writeBuffer_[j] = j;
        }

        /* Write full page of data */
        result = Flash_writeSync(flash, startOffset_ + i, FlashWriteROBuffer_,
                                 remainingSize);
        TEST_ASSERT_EQUAL(FlashSuccess, result);
      }
    }

    for (i = 0; i < info->ulPageSize; i += FlashDefaultMaxBufferSize) {
      /* Less the a full buffer left? */
      int remainingSize =
          ((i + FlashDefaultMaxBufferSize) > info->ulPageSize)
              ? (info->ulPageSize - i)
              : FlashDefaultMaxBufferSize;

      if (remainingSize > 0) {
        /* Read back the data written */
        result = Flash_readSync(flash, startOffset_ + i, &readBuffer_[0],
                                remainingSize);
        TEST_ASSERT_EQUAL(FlashSuccess, result);

        /* Verify the data is erased. */
        for (i = 0; i < remainingSize; i++) {
          if (readBuffer_[i] != FlashWriteROBuffer_[i]) {
            TEST_ASSERT_MESSAGE(0, "Data was NOT erased");
          }
        }
      }
    }
  }

  /* Close flash handle */
  result = flash_close(flash);
  TEST_ASSERT_EQUAL(FlashSuccess, result);
}

/**
 * @brief Test Function to test fuzzing of flash_open and flash_close.
 *
 */
TEST(TestFlash, FlashOpenCloseFuzz) {
  Flash flash, flashTmp;
  int result;

  /* Open the flash Instance_ */
  flash = flash_open(Instance_);
  TEST_ASSERT_NOT_EQUAL(NULL, flash);

  if (TEST_PROTECT()) {
    /* open the same intance twice */
    flashTmp = flash_open(Instance_);
    TEST_ASSERT_EQUAL(NULL, flashTmp);

    /* Open flash to initialize hardware with invalid handle. */
    flashTmp = flash_open(FLASH_INVALID_HANDLE);
    TEST_ASSERT_EQUAL(NULL, flashTmp);
  }

  /* Close with valid handle */
  result = flash_close(flash);
  TEST_ASSERT_EQUAL(FlashSuccess, result);

  /* Close with stale handle */
  result = flash_close(flash);
  TEST_ASSERT_EQUAL(FLASH_INVALID_VALUE, result);
}

/**
 * @brief Test Function to test fuzzing of flash_getinfo
 *
 */
TEST(TestFlash, FlashGetInfoFuzz) {
  FlashInfo *info;

  /* Close with stale handle */
  info = flash_getinfo(NULL);
  TEST_ASSERT_EQUAL(NULL, info);
}

/**
 * @brief Test Function to test fuzzing of flash_ioctl
 *
 */
TEST(TestFlash, FlashIoctlFuzz) {
  Flash flash;
  int result;
  int lStatus;

  /* Call ioctl with null handle */
  result = flash_ioctl(NULL, eGetFlashStatus, (void *const) & lStatus);
  TEST_ASSERT_EQUAL(FLASH_INVALID_VALUE, result);

  /* Open the flash Instance_ */
  flash = flash_open(Instance_);
  TEST_ASSERT_NOT_EQUAL(NULL, flash);

  if (TEST_PROTECT()) {
    /* Call ioctl with handle, but invalid enum */
    result = flash_ioctl(flash, -1, NULL);
    TEST_ASSERT_EQUAL(FLASH_INVALID_VALUE, result);
  }

  /* Close flash handle */
  result = flash_close(flash);
  TEST_ASSERT_EQUAL(FlashSuccess, result);
}

/**
 * @brief Test Function to test fuzzing of flash_erase
 *
 */
TEST(TestFlash, FlashEraseFuzz) {
  Flash flash;
  int result;

  /* Erase one sector with NULL handle*/
  result =
      flash_erase_sectors(NULL, startOffset_, FLASH_DEFAULT_PARTIAL_PAGE_SIZE);
  TEST_ASSERT_EQUAL(FLASH_INVALID_VALUE, result);

  /* Open the flash Instance_ */
  flash = flash_open(Instance_);
  TEST_ASSERT_NOT_EQUAL(NULL, flash);

  if (TEST_PROTECT()) {
    /* Erase one sector with invalid start address*/
    result = flash_erase_sectors(flash, -1, FLASH_DEFAULT_PARTIAL_PAGE_SIZE);
    TEST_ASSERT_EQUAL(FLASH_INVALID_VALUE, result);

    /* Erase one sector with invalid length*/
    result = flash_erase_sectors(flash, startOffset_, -1);
    TEST_ASSERT_EQUAL(FLASH_INVALID_VALUE, result);
  }

  /* Close flash handle */
  result = flash_close(flash);
  TEST_ASSERT_EQUAL(FlashSuccess, result);
}

/**
 * @brief Test Function to test fuzzing of flash_write
 *
 */
TEST(TestFlash, FlashWriteFuzz) {
  Flash flash;
  int result;

  /* Write partial page with invalid handle */
  result = Flash_writeSync(NULL, startOffset_, &writeBuffer_[0],
                           FLASH_DEFAULT_PARTIAL_PAGE_SIZE);
  TEST_ASSERT_EQUAL(FLASH_INVALID_VALUE, result);

  /* Open the flash Instance_ */
  flash = flash_open(Instance_);
  TEST_ASSERT_NOT_EQUAL(NULL, flash);

  if (TEST_PROTECT()) {
    /* Write partial page with invalid start address */
    result = Flash_writeSync(flash, -1, &writeBuffer_[0],
                             FLASH_DEFAULT_PARTIAL_PAGE_SIZE);
    TEST_ASSERT_EQUAL(FLASH_INVALID_VALUE, result);

    /* Write partial page with invalid buffer */
    result = Flash_writeSync(flash, startOffset_, NULL,
                             FLASH_DEFAULT_PARTIAL_PAGE_SIZE);
    TEST_ASSERT_EQUAL(FLASH_INVALID_VALUE, result);
  }

  /* Close flash handle */
  result = flash_close(flash);
  TEST_ASSERT_EQUAL(FlashSuccess, result);
}

TEST(TestFlash, FlashReadFuzz) {
  Flash flash;
  int result;

  /* Read partial page with invalid handle */
  result = Flash_readSync(NULL, startOffset_, &readBuffer_[0],
                          FLASH_DEFAULT_PARTIAL_PAGE_SIZE);
  TEST_ASSERT_EQUAL(FLASH_INVALID_VALUE, result);

  /* Open the flash Instance_ */
  flash = flash_open(Instance_);
  TEST_ASSERT_NOT_EQUAL(NULL, flash);

  if (TEST_PROTECT()) {
    /* Read partial page with invalid start address */
    result = Flash_readSync(flash, -1, &readBuffer_[0],
                            FLASH_DEFAULT_PARTIAL_PAGE_SIZE);
    TEST_ASSERT_EQUAL(FLASH_INVALID_VALUE, result);

    /* Read partial page with invalid buffer */
    result = Flash_readSync(flash, startOffset_, NULL,
                            FLASH_DEFAULT_PARTIAL_PAGE_SIZE);
    TEST_ASSERT_EQUAL(FLASH_INVALID_VALUE, result);
  }

  /* Close flash handle */
  result = flash_close(flash);
  TEST_ASSERT_EQUAL(FlashSuccess, result);
}
