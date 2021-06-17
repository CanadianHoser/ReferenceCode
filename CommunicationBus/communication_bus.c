#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "communication_bus.h"

/*
You are given the request to do initial bring-up for a communication bus called BUS0. 
This bus is being used in receive mode, and it receives intermittent ASCII text. 
In order to test the functionality, the project it to take the ASCII received over the BUS0 and and print it out in your low-priority task. 
You should strive to not miss any data being sent out over the bus or printed. 
Use the provided API below, create, or edit any code provided to accomplish your goal.
*/

// ; Excerpt from startup.s
// ; Interrupts
// ...
// DCD      BUS0_IRQHandler                  ;   // BUS0 Interrupt Handler
// ...


//////////////////////////
// BUS0 HAL API
busname_t BUS0 = {0};

uint8_t buffer[BUFSIZE];
uint8_t *bufEnd = buffer;  // Write 
uint8_t *bufStart = buffer;  // Read

// Handles the state logic and low level registers settings for a bus
// Especially useful at reseting logic after receiving an interrupt
void HAL_BUSx_Statehandler(busname_t *bus);

// Returns the count of the RX FIFO for the bus [0~8 Bytes]
uint8_t HAL_BUSx_GetRxCount(busname_t *bus);

// Copies bytes sitting in RX FIFO to the provided buffer, up to size provided
// returns bytes copied
uint8_t HAL_BUSx_GetBytes(busname_t *bus, 
                          uint8_t *buffer, 
                          uint8_t size);

// IRQ in the interrupt vector
void BUS0_IRQHandler(void){
    uint8_t byteCount;
    uint8_t bytesSaved;
    
    byteCount = HAL_BUSx_GetRxCount(BUS0);
    if (byteCount) {
        bytesSaved = HAL_BUSx_GetBytes(BUS0, bufEnd, byteCount);
        buffer[bytesSaved] ='\0';
        bufEnd += bytesSaved+1;
        xTaskSignal(DATAREADY);
    }
    HAL_BUSx_Statehandler(BUS0);
}



//////////////////////////
// User Code Below

// uint32_t signal = 0;

void vTaskBus0( void * pvParameters )
{
    for (;;) {
        if (xTaskWait(DATAREADY, TIMEOUT))
        {
            // Race condition if just reset bufEnd
            uint8_t localBufEnd = bufEnd;
            magicPrintf(bufStart);
            bufStart = localBufEnd;
        }
    }
}
