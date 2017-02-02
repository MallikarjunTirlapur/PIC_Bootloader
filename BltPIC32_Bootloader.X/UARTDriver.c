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


#include "UARTDriver.h"

/** 
 * @brief function configures UART peripheral.
 */
void driverConfig__UART(void) {  

    U2MODEbits.STSEL = 0;       /* 1 Stopbit */
    U2MODEbits.PDSEL = 0;       /* 8 bits, Parity none */
    U2MODEbits.RTSMD = 0;       /* HW-FlowControl ON */
    U2MODEbits.BRGH = 0;        /* HighSpeed-Mode enabled */
    U2BRG = 11;

    U2STAbits.UTXISEL0 = 0;	/* TxInt if int. buffer and write register empty */
    U2STAbits.UTXISEL1 = 0;
    U2STAbits.ADDEN = 0;	/* Address mode off */
    U2STAbits.UTXEN = 1;        /* Transmitter enable */
    U2STAbits.URXEN = 1;	/* Receiver enable */

    /* Configure UART RX Interrupt */
    IEC1bits.U2RXIE = 1;
    IPC8bits.U2IP = 1;
    
    U2MODEbits.ON = 1;		/* UART Module Enable */    
}

/** 
 * @brief function sends data to the host.
 */
void sendData__UART(uint8_t data) {
        U2TXREG = data;
        while (!U2STAbits.TRMT);
}

/** 
 * @brief The Receive interrupt routine.
 */
void __ISR(_UART_2_VECTOR, IPL1SOFT) _U2RXInterrupt(void)
{
    /*@step check if receive interrupt has hit*/
    if (IFS1bits.U2RXIF) {
        /*@step get the data from the receive register*/
        uart_rxbuffer[uart_rxbuffer_counter++] = U2RXREG;
        /*@step clear the interrupt flag*/
        IFS1bits.U2RXIF = 0;
    }
}