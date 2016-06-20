/* 
 * File:   system.h
 * Author: Mallik
 *
 * Created on 16 May, 2016, 1:32 PM
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


typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned long uint32_t;

void runStateMachine(void);
void initVarConfigPort(void);
void runApplcn(void);

uint16_t firmwareSize;

typedef enum SYSTEM_STATUS_Tag {
    SYS_WAIT_FOR_CONFIG_CMND,
    SYS_CONFIG_FLASH,
    SYS_RCV_FIRMWARE_HEADER,
    SYS_FIRMWARE_UPDATE,
    SYS_RUN_APPLCN,
} SYS_STATE_t;

typedef enum SYSTEM_COM_PROTOCOL_MSG_Tag {
    SYS_CONFIG_CMND_FROM_PROGRMR = 0x01,
    SYS_READY_FOR_FIRMWARE_HEADER,
    SYS_FIRMWARE_HEADER_POS_ACK,
    SYS_FIRMWARE_HEADER_NEG_ACK,
    SYS_SND_NXT_CHUNK,
} SYS_COM_PROTOCL_MSG_t;
    
#ifdef	__cplusplus
}
#endif

#endif	/* SYSTEM_H */

