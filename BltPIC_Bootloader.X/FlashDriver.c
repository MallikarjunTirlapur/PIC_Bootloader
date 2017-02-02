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
 * @file  FlashDriver.c
 * @author Mallikarjun Tirlapur 
 * @date 16 May, 2016, 1:34 PM
 * @brief In this file the Flash load, flash erase functions are defined.
 */


#include "system.h"
#include "FlashDriver.h"
#include "UARTDriver.h"

/** 
 * @brief The function loads binary data from the host in to the flash program memory.
 * the function writes one page (64 bytes) on every call.
 */
void updateFirmware__Flash(void) {
    int size = 0;
    int bufIndex = 0;

    while (size < 8 && bufIndex < 64) {
        for (int bc = 0; bc < 8; bc++) {
            TABLAT = uart_rxbuffer[bufIndex++];
            // Increment source pointer
            // Write data in Table Latch to holding register pointed by
            // ..TBLPTR then increment the TBLPTR
            //_asm tblwt *+ _endasm
            asm(" TBLWTPOSTINC");
            //_asm TBLWTPOSTINC  _endasm
        }
        // Point back into the block to write data
        //_asm tblrd *- _endasm
        asm(" TBLRDPOSTDEC");
        //_asm TBLRDPOSTDEC _endasm
        // Perform the write
        //  ClrWdt();   // make sure watchdog doesn't time out
        INTCONbits.GIE = 0; // disable global interrupt bit
        EECON1bits.WREN = 1;
        EECON1bits.FREE = 0;
        EECON2 = 0X55;
        EECON2 = 0xAA;
        EECON1bits.WR = 1;
        Nop();
        EECON1bits.WREN = 0;
        INTCONbits.GIE = 1;
        // Restore pointer for loading holding registers with next block
        //_asm tblrd *+ _endasm
        asm(" TBLRDPOSTINC");
        //_asm TBLRDPOSTINC _endasm
        // In case the options section requires more than one write.  Note that
        // ..garbage may be written to the area beyond the actual Options.
        size = size + 1;
    }
}

/** 
 * @brief function updates TBLPTRU, TBLPTRH, TBLPTRL registers.
 * The TABLPTR register points the flash memory location to write data.
 */
void updateTablPtr__Flash(void) {
    TBLPTRU = 0x00;
    TBLPTRH = (uint8_t) ((applStartAdd & 0xff00) >> 8);
    TBLPTRL = (uint8_t) (applStartAdd & 0x00ff);
}

/** 
 *@brief function erases application flash memory from the start address to end address.
 */
void erasePage__Flash(void) {
    uint16_t rowAdd = applStartAdd;

    while (rowAdd <= (applStartAdd + (totalNumberOfPacckets * 64))) {
        TBLPTRH = (uint8_t) ((rowAdd & 0xff00) >> 8);
        TBLPTRL = (uint8_t) (rowAdd & 0x00ff);

        EECON1bits.EEPGD = 1;
        EECON1bits.CFGS = 0;
        EECON1bits.WREN = 1;
        EECON1bits.FREE = 1;

        INTCONbits.GIE = 0;

        EECON2 = 0x55;
        EECON2 = 0xAA;

        EECON1bits.WR = 1;

        Nop();
        EECON1bits.WREN = 0;
        INTCONbits.GIE = 1;
        rowAdd = rowAdd + 0x000040;
    }
}

