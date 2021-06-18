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
static uint8_t getBytesBuffer[BUFSIZE];

extern "C" {
extern busname_t BUS0;
extern uint8_t buffer[BUFSIZE];
extern uint8_t *bufEnd;
extern uint8_t *bufStart;

	FAKE_VOID_FUNC(HAL_BUSx_Statehandler, busname_t *);
	FAKE_VALUE_FUNC(uint8_t, HAL_BUSx_GetRxCount, busname_t *);
    FAKE_VOID_FUNC_VARARG(magicPrintf, const char *, ...);
    FAKE_VALUE_FUNC(bool, xTaskWait, uint32_t, uint32_t);
    FAKE_VOID_FUNC(xTaskSignal, uint32_t);

	FAKE_VALUE_FUNC(uint8_t, HAL_BUSx_GetBytes, busname_t *, uint8_t *, uint8_t);
    uint8_t fake_HAL_BUSx_GetBytes(busname_t *bus, uint8_t *buf, uint8_t bytesToRead) {
        uint8_t count = 0;
        while(*bufStart!='\0' && (bytesToRead--)) {
        	*buf++ = *bufStart++;
        	count++;
        }
        return count;
    }
}


TEST_GROUP(comm_bus)
{
    void setup() override
    {
    	FFF_RESET_HISTORY();
        HAL_BUSx_GetBytes_fake.custom_fake = fake_HAL_BUSx_GetBytes;
        buffer[0] = '\0';
        bufStart = buffer;
        bufEnd = buffer;
        bzero(buffer, BUFSIZE);
        bzero(getBytesBuffer, BUFSIZE);
    }

    void teardown() override
    {
    }

    void addCharsToBuffer(const char *bufData)
    {
        while (*bufData != '\0') {
        	*bufEnd++ = (uint8_t)*bufData++;
        }
    }
};

TEST(comm_bus, check_operation)
{
	LONGS_EQUAL(3,3);
}

TEST(comm_bus, getBytes_returns_bytes_added_to_buffer)
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
