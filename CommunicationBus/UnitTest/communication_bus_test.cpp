//
//  communication_bus_tests.cpp
//

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <strings.h>
#include "CppUTest/TestHarness.h"
#include "fff.h"
DEFINE_FFF_GLOBALS;

extern "C" {
#include "communication_bus.h"
}

extern "C" {

    FAKE_VOID_FUNC(HAL_BUSx_Statehandler, busname_t *);
    FAKE_VALUE_FUNC(uint8_t, HAL_BUSx_GetRxCount, busname_t *);
    FAKE_VOID_FUNC_VARARG(magicPrintf, const char *, ...);
    FAKE_VALUE_FUNC(bool, xTaskWait, uint32_t, uint32_t);
    FAKE_VOID_FUNC(xTaskSignal, uint32_t);

    FAKE_VALUE_FUNC(uint8_t, HAL_BUSx_GetBytes, busname_t *, uint8_t *, uint8_t);

    uint8_t fake_HAL_BUSx_GetBytes(busname_t *bus, uint8_t *buf, uint8_t bytesToRead);
    void addCharsToBuffer(const char *bufData);
    void initBytesBuffer(void);
}


TEST_GROUP(comm_bus)
{
    void setup() override
    {
    	FFF_RESET_HISTORY();
        HAL_BUSx_GetBytes_fake.custom_fake = fake_HAL_BUSx_GetBytes;
        bufStart = buffer;
        bufEnd = buffer;
        initBytesBuffer();
        bzero(buffer, BUFSIZE);
    }

    void teardown() override
    {
    }
};

TEST(comm_bus, check_operation)
{
	LONGS_EQUAL(3,3);
}

TEST(comm_bus, fake_getBytes_returns_bytes_added_to_buffer)
{
    uint8_t byteCount;
    uint8_t myBuf[16] = {0};
    addCharsToBuffer("abc");
    byteCount = HAL_BUSx_GetBytes(&BUS0, myBuf, 16);
    LONGS_EQUAL(3, byteCount);
    STRCMP_EQUAL("abc", (char *)myBuf);
}

TEST(comm_bus, irq_handler_adds_bytes_to_buffer)
{
    addCharsToBuffer("abcdef");
    HAL_BUSx_GetRxCount_fake.return_val = 3;
    BUS0_IRQHandler();
    STRCMP_EQUAL("abc", (char *)buffer);
}

TEST(comm_bus, multiple_irq_handler_calls_does_not_overwrite_any_unhandled_entries)
{
    addCharsToBuffer("abcdef");
    HAL_BUSx_GetRxCount_fake.return_val = 3;
    BUS0_IRQHandler();
    HAL_BUSx_GetRxCount_fake.return_val = 3;
    BUS0_IRQHandler();
    STRCMP_EQUAL("abc", (char *)&buffer[0]);
    CHECK_TRUE('\0' == buffer[3]);
    STRCMP_EQUAL("def", (char *)&buffer[4]);
}

TEST(comm_bus, printBuffer_in_userSpace_updates_read_pointer)
{
    addCharsToBuffer("abcdef");
    HAL_BUSx_GetRxCount_fake.return_val = 3;
    BUS0_IRQHandler();
    POINTERS_EQUAL(&buffer[4], bufEnd);
    POINTERS_EQUAL(&buffer[0], bufStart);
    // Buffer has data in it, now read
    printBuffer();
    POINTERS_EQUAL(&buffer[4], bufStart);
}
