/*
 * communication_bus_mocks.c
 *
 *  Created on: Jun 17, 2021
 *      Author: whein
 */

#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "communication_bus.h"

static uint8_t getBytesBuffer[BUFSIZE];
static uint8_t *getBytesBufEnd;
static uint8_t *getBytesBufStart;
static bool outputEnable = false;
char outputBuffer[BUFSIZE];

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

void enable_magicPrintf(void) {outputEnable = true;}
void disable_magicPrintf(void) {outputEnable = false;}
void fake_magicPrintf(const char *format, va_list args)
{
	vsprintf(outputBuffer, format, args);
    if (outputEnable)
    	fprintf(stdout, "%s\n", outputBuffer);
}
