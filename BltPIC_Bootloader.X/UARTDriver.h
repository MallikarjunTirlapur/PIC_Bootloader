/*
 * The MIT License
 *
 * Copyright(c) 2016 Mallikarjun Tirlapur.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/**
 * @file UARTDriver.h
 * @author Mallikarjun Tirlapur 
 * @date 14 May, 2016
 * @brief File contains the declaration of UART functions and uart_rxbuffer array.
 */

#ifndef UARTCONFIGDRIVER_H
#define	UARTCONFIGDRIVER_H

#include "system.h"


#ifdef	__cplusplus
extern "C" {
#endif



uint8_t uart_rxbuffer[NVM_PAGE_SIZE]; /**< uart receive buffer of size NVM page. */
uint8_t uart_rxbuffer_counter; /**< uart receive buffer counter. */
  

void driverConfig__UART(void);

void sendData__UART(uint8_t data);


#ifdef	__cplusplus
}
#endif

#endif	/* UARTCONFIGDRIVER_H */

