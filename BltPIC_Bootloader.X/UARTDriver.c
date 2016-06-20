/*
 * File:   UARTDriver.c
 * Author: Mallik
 *
 * Created on 14 May, 2016, 11:38 PM
 */


#include "system.h"
#include "UARTDriver.h"


void driverConfig__UART(void) {

    RCONbits.IPEN = 1; // ENABLE interrupt priority
    INTCONbits.GIE = 1; // ENABLE interrupts
    INTCONbits.PEIE = 1; // ENable peripheral interrupts.
    PIE1bits.RCIE = 1; // ENABLE USART receive interrupt

    TXSTA = 0;
    RCSTA = 0;

    TXSTAbits.BRGH = 0;

    SPBRG = 25; //at FOSC 16MHz

    RCSTAbits.SPEN = 1;
    TXSTAbits.TXEN = 1;
    RCSTAbits.CREN = 1;
}

void sendData__UART(uint8_t data) {
    TXREG = data;
    while (!TXSTAbits.TRMT);
}
// High priority interrupt routine

static void interrupt InterruptHandlerHigh(void) {
    if (PIR1bits.RCIF) {
        uart_rxbuffer[uart_rxbuffer_counter++] = RCREG;
        PIR1bits.RCIF = 0;
    }
}