/*
 * communication_bus_mocks.c
 *
 *  Created on: Jun 17, 2021
 *      Author: whein
 */

#include <stdint.h>
#include <string.h>
#include "communication_bus.h"

static uint8_t getBytesBuffer[BUFSIZE];
static uint8_t *getBytesBufEnd;
static uint8_t *getBytesBufStart;

uint8_t fake_HAL_BUSx_GetBytes(busname_t *bus, uint8_t *buf, uint8_t bytesToRead)
{
    uint8_t count = 0;
    while(*getBytesBufStart!='\0' && (bytesToRead--)) {
    	*buf++ = *getBytesBufStart++;
    	count++;
    }
    return count;
}

void initBytesBuffer(void)
{
        getBytesBufStart = getBytesBuffer;
        getBytesBufEnd = getBytesBuffer;
        bzero(getBytesBuffer, BUFSIZE);
}

void addCharsToBuffer(const char *bufData)
{
    while (*bufData != '\0') {
    	*getBytesBufEnd++ = (uint8_t)*bufData++;
    }
}
