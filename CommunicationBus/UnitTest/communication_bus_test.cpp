//
// communication_bus_tests.cpp
//
// These tests make use of CppUTest to execute: http://cpputest.github.io
// See the FFF framework for the execution/declaration of fakes: https://github.com/meekrosoft/fff
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
#include "communication_bus_mocks.h"
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
        magicPrintf_fake.custom_fake = fake_magicPrintf;
        disable_magicPrintf();
    }

    void teardown() override
    {
    }
};

TEST(comm_bus, fake_getBytes_returns_count_of_bytes_added_to_buffer)
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
    // buffer: 'a' 'b', 'c', '\0'
    HAL_BUSx_GetRxCount_fake.return_val = 3;
    BUS0_IRQHandler();
    // buffer: 'a' 'b', 'c', '\0', 'd', 'e', 'f', '\0'
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

TEST(comm_bus, interleaving_printBuffer_only_prints_last_read)
{
    addCharsToBuffer("abcdef");
    HAL_BUSx_GetRxCount_fake.return_val = 3;
    BUS0_IRQHandler();
    // buffer: 'a' 'b', 'c', '\0'
    POINTERS_EQUAL(&buffer[4], bufEnd);
    POINTERS_EQUAL(&buffer[0], bufStart);
    printBuffer();
    HAL_BUSx_GetRxCount_fake.return_val = 3;
    BUS0_IRQHandler();
    printBuffer();
    STRCMP_EQUAL("def", outputBuffer);
    POINTERS_EQUAL(bufStart, bufEnd);
}
