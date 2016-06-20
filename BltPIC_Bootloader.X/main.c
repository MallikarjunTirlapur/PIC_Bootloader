/*
 * File:   main.c
 * Author: mallik
 *
 * Created on 14 May, 2016, 11:04 PM
 */


#include <xc.h>
#include "system.h"
#include "FlashDriver.h"
#include "UARTDriver.h"

void main(void) {
//    runApplcn();
    initVarConfigPort();
    driverConfig__UART();
    runStateMachine();
}

void initVarConfigPort(void) {
    PORTB = 0;
    ANSEL0 = 0;
    ANSEL1 = 0;

    TRISCbits.RC6 = 0; //TX pin set as output
    TRISCbits.RC7 = 1; //RX pin set as input

    uart_rxbuffer_counter = 0;
}

void runApplcn(void) {
	void (*applcFtr)(void);
	applcFtr = (void (*)(void))0x000400;
	applcFtr();
    
//    void *ptr = (void *)0x002000;  // a random memory address
//    goto *ptr; 
}

void runStateMachine(void) {
    SYS_STATE_t currentState = SYS_WAIT_FOR_CONFIG_CMND;

    while (1) {
        switch (currentState) {

            case SYS_WAIT_FOR_CONFIG_CMND:
                if (!uart_rxbuffer_counter || (uart_rxbuffer [0] != SYS_CONFIG_CMND_FROM_PROGRMR)) {
                    currentState = SYS_WAIT_FOR_CONFIG_CMND;
                } else {
                    sendData__UART(SYS_READY_FOR_FIRMWARE_HEADER);
                    uart_rxbuffer_counter = 0;
                    currentState = SYS_CONFIG_FLASH;
                }
                break;

            case SYS_CONFIG_FLASH:
                updateTablPtr__Flash();
                eraseSeq__Flash();
                currentState = SYS_RCV_FIRMWARE_HEADER;
                break;

            case SYS_RCV_FIRMWARE_HEADER:
            {
                uint8_t cmpArr[4];
                memset(cmpArr, 0x55, 4);
                if (uart_rxbuffer_counter < 5) {
                    currentState = SYS_RCV_FIRMWARE_HEADER;
                } else {
                    if (memcmp(cmpArr, uart_rxbuffer, 4)) {
                        sendData__UART(SYS_FIRMWARE_HEADER_NEG_ACK);
                    } else {
                        firmwareSize = uart_rxbuffer[4];
                        uart_rxbuffer_counter = 0;
                        sendData__UART(SYS_FIRMWARE_HEADER_POS_ACK);
                        currentState = SYS_FIRMWARE_UPDATE;
                    }

                }
            }
                break;

            case SYS_FIRMWARE_UPDATE:
                if (uart_rxbuffer_counter < NVM_PAGE_SIZE) {
                    currentState = SYS_FIRMWARE_UPDATE;
                } else {
                    uart_rxbuffer_counter = 0;
                    updateFirmware__Flash();
                    if (--firmwareSize > 0) {
                        sendData__UART(SYS_SND_NXT_CHUNK);
                    } else {
                        currentState = SYS_RUN_APPLCN;
                    }
                }
                break;

            case SYS_RUN_APPLCN:
                runApplcn();
                currentState = SYS_WAIT_FOR_CONFIG_CMND;
                break;
        }
    }
}