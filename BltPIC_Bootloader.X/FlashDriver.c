/*
 * File:   FlashWrite.c
 * Author: Mallik
 *
 * Created on 16 May, 2016, 1:34 PM
 */


#include "system.h"
#include "FlashDriver.h"
#include "UARTDriver.h"


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

void updateTablPtr__Flash(void){
    TBLPTRU = OPTIONS_START_U;
    TBLPTRH = OPTIONS_START_H;
    TBLPTRL = OPTIONS_START_L;
}

void eraseSeq__Flash(void) {
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
}

