//
//  communication_bus_tests.cpp
//

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include "CppUTest/TestHarness.h"
#include "fff.h"
DEFINE_FFF_GLOBALS;

extern "C" {
#include "communication_bus.h"

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
    uint8_t fake_HAL_BUSx_GetBytes(busname_t *bus, uint8_t *buf, uint8_t bytesRead) {
        sprintf((char *)buf, "abc");
        return(3);
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
    }

    void teardown() override
    {
    }
};

TEST(comm_bus, check_operation)
{
	LONGS_EQUAL(3,3);
}

TEST(comm_bus, check_fake_for_HAL_BUSx_GetBytes)
{
    uint8_t byteCount;
    uint8_t myBuf[16] = {0};
    HAL_BUSx_GetRxCount_fake.return_val = 3;
    byteCount = HAL_BUSx_GetBytes(&BUS0, myBuf, 16);
    LONGS_EQUAL(3, byteCount);
    STRCMP_EQUAL("abc", (char *)myBuf);
}
