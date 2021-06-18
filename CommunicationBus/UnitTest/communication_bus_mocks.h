/*
 * communication_bus_mocks.h
 *
 *  Created on: Jun 17, 2021
 *      Author: whein
 */

#ifndef COMMUNICATIONBUS_UNITTEST_COMMUNICATION_BUS_MOCKS_H_
#define COMMUNICATIONBUS_UNITTEST_COMMUNICATION_BUS_MOCKS_H_

#include <stdint.h>
#include <stdbool.h>
#include "communication_bus.h"

    FAKE_VOID_FUNC(HAL_BUSx_Statehandler, busname_t *);
    FAKE_VALUE_FUNC(uint8_t, HAL_BUSx_GetRxCount, busname_t *);
    FAKE_VOID_FUNC_VARARG(magicPrintf, const char *, ...);
    FAKE_VALUE_FUNC(bool, xTaskWait, uint32_t, uint32_t);
    FAKE_VOID_FUNC(xTaskSignal, uint32_t);
    FAKE_VALUE_FUNC(uint8_t, HAL_BUSx_GetBytes, busname_t *, uint8_t *, uint8_t);

    uint8_t fake_HAL_BUSx_GetBytes(busname_t *bus, uint8_t *buf, uint8_t bytesToRead);
    void addCharsToBuffer(const char *bufData);
    void initBytesBuffer(void);



#endif /* COMMUNICATIONBUS_UNITTEST_COMMUNICATION_BUS_MOCKS_H_ */
