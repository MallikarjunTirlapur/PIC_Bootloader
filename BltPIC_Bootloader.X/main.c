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
 * @file  main.c
 * @author Mallikarjun Tirlapur 
 * @date 14 May, 2016, 11:04 PM
 * @brief In this file the functions are defined and processed the commands from the host.
 */


#include <xc.h>
#include "system.h"
#include "FlashDriver.h"
#include "UARTDriver.h"

/** 
 * @brief entry point of the system
 * init functions are called from here.
 */
void main(void) {
    initVarConfigPort();
    driverConfig__UART();
    runStateMachine();
}

/** 
 * @brief function configures and initializes IO ports.
 */
void initVarConfigPort(void) {
    PORTB = 0;
    /*@step configure ports to digital I/O*/
    ANSEL0 = 0;
    ANSEL1 = 0;

    /*@step TX pin set as output*/
    TRISCbits.RC6 = 0;

    /*@step RX pin set as input*/
    TRISCbits.RC7 = 1;
}

/** 
 * @brief PC is updated with the new pointer to execute loaded binary from the application flash program memory.
 */
void runApplcn(void) {
    void (*applcFtr)(void);
    applcFtr = (void (*)(void))applStartAdd;
    applcFtr();
}

/** 
 * @brief function executes different states of the system.
 *  SYS_SND_ADDRS_RANGE      -          Send the PIC programmable start and end address of the program memory
 *  SYS_WAIT_FOR_START_CMND  -          The Start command triggers system to start the process and initializes buffer counter
 *  SYS_RCV_FIRMWARE_HEADER  -          Get the total packet count from the host and applcn start address
 *  SYS_CONFIG_FLASH         -          Erase application program area and update table pointer
 *  SYS_FIRMWARE_UPDATE      -          Receive and put the binary chunks in the flash memory
 *  SYS_RUN_APPLCN           -          Run the application
 */
void runStateMachine(void) {

    uint8_t i;
    SYS_STATE_t currentState = SYS_SND_ADDRS_RANGE;


    while (1) {
        switch (currentState) {
            
            case SYS_SND_ADDRS_RANGE:
                if (!uart_rxbuffer_counter || (uart_rxbuffer [0] != SYS_ADDRESS_RANGE_RQT)) {
                    currentState = SYS_SND_ADDRS_RANGE;
                } else {
                    sendData__UART(progMemRange[0]); //T
                    sendData__UART(progMemRange[1]); //NVM_PAGE_SIZE
                    for (i = 2; i <= (progMemRange[1] + 1); i++) {
                        sendData__UART(progMemRange[i]); //V
                    }
                    uart_rxbuffer_counter = 0;
                    currentState = SYS_WAIT_FOR_START_CMND;
                }
                break;

            case SYS_WAIT_FOR_START_CMND:
                if (!uart_rxbuffer_counter || (uart_rxbuffer [0] != SYS_START_CMND_FROM_PROGRMR)) {
                    currentState = SYS_WAIT_FOR_START_CMND;
                } else {
                    sendData__UART(SYS_READY_FOR_FIRMWARE_HEADER);
                    uart_rxbuffer_counter = 0;
                    currentState = SYS_RCV_FIRMWARE_HEADER;
                }
                break;

            case SYS_RCV_FIRMWARE_HEADER:
                if (uart_rxbuffer_counter < 3) {
                    currentState = SYS_RCV_FIRMWARE_HEADER;
                } else {

                    /*@step get the packet counts from the host*/
                    totalNumberOfPacckets = uart_rxbuffer[0];

                    /*@step get the applcn start address*/
                    applStartAdd += uart_rxbuffer[2];
                    applStartAdd = applStartAdd << 8;
                    applStartAdd += uart_rxbuffer[1];

                    uart_rxbuffer_counter = 0;
                    currentState = SYS_CONFIG_FLASH;
                }

                break;

            case SYS_CONFIG_FLASH:
                /*@step erase applcn program memory page*/
                erasePage__Flash();

                /*@step Update flash table pointer*/
                updateTablPtr__Flash();
                sendData__UART(SYS_FIRMWARE_HEADER_POS_ACK);
                currentState = SYS_FIRMWARE_UPDATE;
                break;

            case SYS_FIRMWARE_UPDATE:
                if (uart_rxbuffer_counter < NVM_PAGE_SIZE) {
                    currentState = SYS_FIRMWARE_UPDATE;
                } else {
                    uart_rxbuffer_counter = 0;

                    /*@step load firmware in the flash*/
                    updateFirmware__Flash();
                    if (--totalNumberOfPacckets > 0) {
                        sendData__UART(SYS_SND_NXT_CHUNK);
                    } else {
                        currentState = SYS_RUN_APPLCN;
                    }
                }
                break;

            case SYS_RUN_APPLCN:
                uart_rxbuffer_counter = 0;
                /*@step execute applcn code by setting the PC to applcn reset vector*/
                runApplcn();
                currentState = SYS_WAIT_FOR_START_CMND;
                break;
        }
    }
}
