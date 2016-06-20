/* 
 * File:   UARTDriver.h
 * Author: Mallik
 *
 * Created on 14 May, 2016, 11:42 PM
 */

#ifndef UARTCONFIGDRIVER_H
#define	UARTCONFIGDRIVER_H

#include "system.h"


#ifdef	__cplusplus
extern "C" {
#endif

//#define sysFreq 16000000
//#define baudRate 9600
//#define baudRateGen ((sysFreq - baudRate*64) / (baudRate*64))
#define NVM_PAGE_SIZE 64

uint8_t uart_rxbuffer[NVM_PAGE_SIZE];   
uint8_t uart_rxbuffer_counter;
    
void driverConfig__UART(void);
void sendData__UART(uint8_t data);


#ifdef	__cplusplus
}
#endif

#endif	/* UARTCONFIGDRIVER_H */

