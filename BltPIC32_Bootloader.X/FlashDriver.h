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
 * @file FlashDriver.h
 * @author Mallikarjun Tirlapur 
 * @date 16 May, 2016
 * @brief File contains the declaration of flash functions.
 */
#ifndef FLASHWRITE_H
#define	FLASHWRITE_H

#include "system.h"

#ifdef	__cplusplus
extern "C" {
#endif



#define NVMOP_WORD_PGM          0x4001      // Word program operation
#define NVMOP_PAGE_ERASE        0x4004      // Page erase operation
#define NVMOP_ROW_PGM           0x4003      // Row program operation
#define NVMOP_NOP               0x4000      // NOP operation

    

#define APP_FLASH_MAX_SIZE     (APP_FLASH_END_ADDRES - APP_FLASH_BASE_ADDRESS + 1)

#define USER_APP_RESET_ADDRESS 	(0x9d006000 + 0x1000 + 0x970)
    
void updateFirmware__Flash(void);
void eraseSeq__Flash(void);

#ifdef	__cplusplus
}
#endif

#endif	/* FLASHWRITE_H */

