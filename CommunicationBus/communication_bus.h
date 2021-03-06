/*
 * communication_bus.h
 *
 *  Created on: Jun 17, 2021
 *      Author: whein
 */

#ifndef COMMUNICATIONBUS_COMMUNICATION_BUS_H_
#define COMMUNICATIONBUS_COMMUNICATION_BUS_H_

#include <stdbool.h>

// typedef int DCD;
typedef int busname_t;

#define BUFSIZE 256
#define DATAREADY 1
#define TIMEOUT 50

#ifdef UNIT_TEST
extern busname_t BUS0;
extern uint8_t buffer[BUFSIZE];
extern uint8_t *bufEnd;
extern uint8_t *bufStart;
#endif

//////////////////////////
// TASK API

// Sleeps the thread for xWait_ms millisconds or until it receives a notification
// that matches xSignal
// returns true if it woke from notifications, whereas false for timeout
bool xTaskWait( uint32_t xSignal,
                uint32_t xWait_ms);

// Notifies any thread listening to the xSignal
void xTaskSignal(uint32_t xSignal);


//////////////////////////
// PRINTING API

// Like the standard printf
void magicPrintf( const char * format, ... );
void printBuffer(void);

void BUS0_IRQHandler(void);
void vTaskBus0( void * pvParameters ) __attribute__((noreturn));


#endif /* COMMUNICATIONBUS_COMMUNICATION_BUS_H_ */
