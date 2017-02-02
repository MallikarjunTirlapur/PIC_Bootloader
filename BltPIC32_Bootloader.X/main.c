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
 * @date 29 Aug, 2016, 11:04 PM
 * @brief In this file the functions are defined and processed the commands from the host.
 */


#include "system.h"
#include "FlashDriver.h"
#include "UARTDriver.h"


#pragma config FPLLMUL  = MUL_20        // PLL Multiplier
#pragma config UPLLIDIV = DIV_2         // USB PLL Input Divider
#pragma config FPLLIDIV = DIV_2         // PLL Input Divider
#pragma config FPLLODIV = DIV_2         // PLL Output Divider
#pragma config FPBDIV   = DIV_1         // Peripheral Clock divisor
#pragma config FWDTEN   = OFF           // Watchdog Timer
#pragma config WDTPS    = PS16384       // Watchdog Timer Postscale
#pragma config FCKSM    = CSDCMD        // Clock Switching & Fail Safe Clock Monitor
#pragma config OSCIOFNC = ON            // CLKO Enable
#pragma config POSCMOD  = OFF           // Primary Oscillator
#pragma config IESO     = OFF           // Internal/External Switch-over
#pragma config FSOSCEN  = OFF           // Secondary Oscillator Enable (KLO was off)
#pragma config FNOSC    = FRCPLL        // Oscillator Selection
#pragma config CP       = OFF           // Code Protect
#pragma config BWP      = OFF           // Boot Flash Write Protect
#pragma config PWP      = OFF           // Program Flash Write Protect
#pragma config ICESEL   = ICS_PGx2      // ICE/ICD Comm Channel Select


uint16_t progMemRange[] = {SYS_ADDRESS_RANGE_TAG, BYTE_ROW_SIZE, 
                         (uint8_t)((APP_FLASH_END_ADDRES & 0xff000000) >> 24),
                         (uint8_t)((APP_FLASH_END_ADDRES & 0x00ff0000) >> 16),
                         (uint8_t)((APP_FLASH_END_ADDRES & 0x0000ff00) >> 8),
                         (uint8_t)(APP_FLASH_END_ADDRES & 0x000000ff),
                         (uint8_t)((APP_FLASH_BASE_ADDRESS & 0xff000000) >> 24),
                         (uint8_t)((APP_FLASH_BASE_ADDRESS & 0x00ff0000) >> 16),
                         (uint8_t)((APP_FLASH_BASE_ADDRESS & 0x0000ff00) >> 8),
                         (uint8_t)(APP_FLASH_BASE_ADDRESS & 0x000000ff)};


void main(void) {
    SYSTEMConfigPerformance(SYS_FREQ);
    initVarConfigPort();
    driverConfig__UART();
    runStateMachine();
}

void initVarConfigPort(void) {

    AD1PCFG = 0xFFFF;
    TRISFbits.TRISF5 = 0; //TX pin set as output
    TRISFbits.TRISF4 = 1; //RX pin set as input
    LATFbits.LATF5 = 1;


    uart_rxbuffer_counter = 0;
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
                    sendData__UART((uint8_t) progMemRange[0]); //T
                    sendData__UART((uint8_t) progMemRange[1]); //BYTE_ROW_SIZE
                    for (i = 2; i <= (progMemRange[1] + 1); i++) {
                        sendData__UART((uint8_t) progMemRange[i]); //V
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
                if (uart_rxbuffer_counter < 5) {
                    currentState = SYS_RCV_FIRMWARE_HEADER;
                } else {

                    /*@step get the packet counts from the host*/
                    totalNumberOfPacckets = uart_rxbuffer[0];

                    /*@step get the applcn start address*/
                    applStartAdd += uart_rxbuffer[4];
                    applStartAdd = applStartAdd << 8;
                    applStartAdd += uart_rxbuffer[3];
                    applStartAdd = applStartAdd << 8;
                    applStartAdd += uart_rxbuffer[2];
                    applStartAdd = applStartAdd << 8;
                    applStartAdd += uart_rxbuffer[1];

                    uart_rxbuffer_counter = 0;
                    currentState = SYS_CONFIG_FLASH;
                }

                break;

            case SYS_CONFIG_FLASH:
                /*@step erase applcn program memory page*/
                eraseSeq__Flash();

                sendData__UART(SYS_FIRMWARE_HEADER_POS_ACK);
                currentState = SYS_FIRMWARE_UPDATE;
                break;

            case SYS_FIRMWARE_UPDATE:
                if (uart_rxbuffer_counter < BYTE_ROW_SIZE) {
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
