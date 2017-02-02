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
 * @file system.h
 * @author Mallikarjun Tirlapur 
 * @date 16 May, 2016
 * @brief The functions are declared and defined state enumerations.
 */

#ifndef SYSTEM_H
#define	SYSTEM_H

#ifdef	__cplusplus
extern "C" {
#endif
#include <xc.h>
#include <p18f4431.h>
#include <string.h>
#include <stdlib.h>

#pragma config OSC = HS
#pragma config WDTEN = OFF
#pragma config MCLRE = ON
#pragma config CPD = OFF
#pragma config BOREN = ON
#pragma config IESO = ON
#pragma config FCMEN = ON
#pragma config STVREN = ON
#pragma config LVP = ON

typedef unsigned char uint8_t; /**< typedef for unsigned char. */
typedef unsigned short uint16_t; /**< typedef for unsigned short. */
typedef unsigned long uint32_t; /**< typedef for unsigned long. */
     
#define NVM_ROW_SIZE 8 /**< NVM each row size 8 bytes. */
#define NVM_PAGE_SIZE 64 /**< NVM each page size 64 bytes. */

#define NVM_START_ADDRESS 0x00 /**< NVM start adress. */
#define NVM_END_ADDRESS 0x3fff /**< NVM end adress. */

uint16_t applStartAdd; /**< Application start address. */

void runStateMachine(void);

void initVarConfigPort(void);

void runApplcn(void);

uint8_t totalNumberOfPacckets; /**< Variable holds the (size of the firmware/64) in bytes. */

/**
 * @brief Enumeral represents different system states.
 */
typedef enum SYSTEM_STATUS_Tag {
    SYS_WAIT_FOR_START_CMND, /**< State where system waits for a start command from the programmer. */
    SYS_CONFIG_FLASH, /**< State where system configs flash */
    SYS_RCV_FIRMWARE_HEADER, /**< State where system waits for firmware header from the programmer */
    SYS_FIRMWARE_UPDATE, /**< State where system loads the binary into the NVM */
    SYS_RUN_APPLCN, /**< State where system jumps to the application */
    SYS_SND_ADDRS_RANGE, /**<State where the system sends the PIC programmable start and end address of the program memory */
} SYS_STATE_t;

/**
 * @brief Enumeral represents handshaking signals between programmer and controller.
 */
typedef enum SYSTEM_COM_PROTOCOL_MSG_Tag {
    SYS_START_CMND_FROM_PROGRMR = 0x01, /**< Config command signal from the programmer. */
    SYS_READY_FOR_FIRMWARE_HEADER, /**< Ready for firmware header signal sent to the programmer. */
    SYS_FIRMWARE_HEADER_POS_ACK, /**< Firmware header positive acknowledgement. */
    SYS_FIRMWARE_HEADER_NEG_ACK, /**< Firmware header negative acknowledgement. */
    SYS_SND_NXT_CHUNK, /**< send next chunk signal. */
    SYS_ADDRESS_RANGE_TAG, /**< send address with this tag. */
    SYS_ADDRESS_RANGE_RQT, /**< Rquest for the address from the host. */
} SYS_COM_PROTOCL_MSG_t;


uint8_t progMemRange[] = {SYS_ADDRESS_RANGE_TAG, NVM_PAGE_SIZE, 
                         (uint8_t)((NVM_END_ADDRESS & 0xff000000) >> 24),
                         (uint8_t)((NVM_END_ADDRESS & 0x00ff0000) >> 16),
                         (uint8_t)((NVM_END_ADDRESS & 0x0000ff00) >> 8),
                         (uint8_t)(NVM_END_ADDRESS & 0x000000ff),
                         (uint8_t)((NVM_START_ADDRESS & 0xff000000) >> 24),
                         (uint8_t)((NVM_START_ADDRESS & 0x00ff0000) >> 16),
                         (uint8_t)((NVM_START_ADDRESS & 0x0000ff00) >> 8),
                         (uint8_t)(NVM_START_ADDRESS & 0x000000ff)};

    
#ifdef	__cplusplus
}
#endif

#endif	/* SYSTEM_H */

