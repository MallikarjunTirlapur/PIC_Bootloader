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
 * @file  UARTDriver.c
 * @author Mallikarjun Tirlapur 
 * @date 14 May, 2016, 11:38 PM
 * @brief In this file the UART config, sendData, ISR functions are defined.
 */


#include "system.h"
#include "UARTDriver.h"

/** 
 * @brief function configures USART peripheral.
 */
void driverConfig__UART(void) {

    RCONbits.IPEN = 1; /*@step ENABLE interrupt priority*/
    INTCONbits.GIE = 1; /*@step ENABLE interrupts*/
    INTCONbits.PEIE = 1; /*@step ENable peripheral interrupts.*/
    PIE1bits.RCIE = 1; /*@step ENABLE USART receive interrupt*/

    TXSTA = 0;
    RCSTA = 0;

    TXSTAbits.BRGH = 0;
    
    /*@step 115200 baudrate is set for USART to communicate with bluetooth module*/
    SPBRG = 25; //at FOSC 16MHz

    /*@step ENABLE USART */
    RCSTAbits.SPEN = 1;
    TXSTAbits.TXEN = 1;
    RCSTAbits.CREN = 1;
}

/** 
 * @brief function sends data to the host.
 */
void sendData__UART(uint8_t data) {
    /*@step load transmit register with the data*/
    TXREG = data;
    /*@step wait till the USART sends out the data*/
    while (!TXSTAbits.TRMT);
}

/** 
 * @brief The High priority interrupt routine.
 */
static void interrupt InterruptHandlerHigh(void) {
    /*@step check if receive interrupt has hit*/
    if (PIR1bits.RCIF) {
        /*@step get the data from the receive register*/
        uart_rxbuffer[uart_rxbuffer_counter++] = RCREG;
        /*@step clear the interrupt flag*/
        PIR1bits.RCIF = 0;
    }
}