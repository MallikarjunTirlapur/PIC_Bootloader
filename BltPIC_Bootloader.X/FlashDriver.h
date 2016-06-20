/* 
 * File:   FlashWrite.h
 * Author: Mallik
 *
 * Created on 16 May, 2016, 2:29 PM
 */

#ifndef FLASHWRITE_H
#define	FLASHWRITE_H

#include "system.h"

#ifdef	__cplusplus
extern "C" {
#endif

#define FLASH_WRITE_START_ADDRESS 0x000000


#define OPTIONS_START_U 0x00
#define OPTIONS_START_H 0x04
#define OPTIONS_START_L 0x00

void updateFirmware__Flash(void);
void eraseSeq__Flash(void);
void updateTablPtr__Flash(void);

#ifdef	__cplusplus
}
#endif

#endif	/* FLASHWRITE_H */

