/******************************************************************************/
/* Files to Include                                                           */
/******************************************************************************/

/* Device header file */
#if defined(__XC16__)
    #include <xc.h>
#elif defined(__C30__)
    #if defined(__PIC24E__)
    	#include <p24Exxxx.h>
    #elif defined (__PIC24F__)||defined (__PIC24FK__)
	#include <p24Fxxxx.h>
    #elif defined(__PIC24H__)
	#include <p24Hxxxx.h>
    #endif
#endif

#include <stdint.h>          /* For uint32_t definition */
#include <stdbool.h>         /* For true/false definition */

#include "system.h"          /* variables/params used by system.c */
#include <libpic30.h>
#include "ds18b20.h"

/******************************************************************************/
/* System Level Functions                                                     */
/*                                                                            */
/* Custom oscillator configuration funtions, reset source evaluation          */
/* functions, and other non-peripheral microcontroller initialization         */
/* functions get placed in system.c                                           */
/*                                                                            */
/******************************************************************************/



/* TODO Add clock switching code if appropriate.  An example stub is below.   */
void ConfigureOscillator(void) {

    //CLKDIVbits.RCDIV = 0b000;   // Postscaler 1:1 -> 8MHz
    /*
     * When COSC<2:0> (OSCCON<14:12>) = 111:
        111 = 31.25 kHz (divide-by-256)
        110 = 125 kHz (divide-by-64)
        101 = 250 kHz (divide-by-32)
        100 = 500 kHz (divide-by-16)
        011 = 1 MHz (divide-by-8)
        010 = 2 MHz (divide-by-4)
        001 = 4 MHz (divide-by-2) (default)
        000 = 8 MHz (divide-by-1)
        When COSC<2:0> (OSCCON<14:12>) = 110:
        111 = 1.95 kHz (divide-by-256)
        110 = 7.81 kHz (divide-by-64)
        101 = 15.62 kHz (divide-by-32)
        100 = 31.25 kHz (divide-by-16)
        011 = 62.5 kHz (divide-by-8)
        010 = 125 kHz (divide-by-4)
        001 = 250 kHz (divide-by-2) (default)
        000 = 500 kHz (divide-by-1)
     */
    CLKDIVbits.RCDIV = 0b000;   
}



// Read EEP
int16_t readEEP(const uint16_t adress) {

    uint16_t addr = adress;

    TBLPAG = 0x7F; // Initialize EE Data page pointer

    return __builtin_tblrdl(addr);
}



/**
 * Ecris 16bits dans l'eeprom (erase + ecriture ensuite)
 *
 * @param address adresse du dword
 * @param data  données à écrire
 */
void writeEEP(uint16_t address, int16_t data) {

    TBLPAG = 0x7F; // Initialize EE Data page pointer

    NVMCON = 0x4058; // Set up NVMCON to erase one word of data EEPROM
    
    __builtin_tblwtl(address, 0xffff);
    asm volatile ("disi #5");
    __builtin_write_NVM(); // Issue Unlock Sequence & Start Write Cycle
    while (NVMCONbits.WR); // wait for last write to complete

    
    // Set up NVMCON to erase one word of data EEPROM
    NVMCON = 0x4004;

    __builtin_tblwtl(address, data); // Write EEPROM data to write latch
    asm volatile ("disi #5"); // Disable Interrupts For 5 Instructions
    __builtin_write_NVM(); // Issue Unlock Sequence & Start Write Cycle
    while (NVMCONbits.WR); // Optional: Poll WR bit to wait for
}

void getRTCCTimeData(int8_t *wDay, int8_t *hour, int8_t *min, int8_t *sec) {

    uint16_t rtcval;

    RCFGCALbits.RTCPTR=0b00;
    rtcval = RTCVAL;

    *min = ((rtcval&0x7000)>>12)*10 + ((rtcval&0x0F00)>>8);
    *sec = ((rtcval&0x0070)>>4)*10 + (rtcval&0x000F);

    RCFGCALbits.RTCPTR=0b01;
    rtcval = RTCVAL;
    *wDay = ((rtcval&0x0700)>>8);
    *hour = ((rtcval&0x0070)>>4)*10 + (rtcval&0x000F);
}



void getRTCCMinSec(int8_t *min, int8_t *sec) {
    
    uint16_t rtcval;

    RCFGCALbits.RTCPTR=0b00;
    rtcval = RTCVAL;

    *min = ((rtcval&0x7000)>>12)*10 + ((rtcval&0x0F00)>>8);
    *sec = ((rtcval&0x0070)>>4)*10 + (rtcval&0x000F);
}



int16_t DS18B20_Convert() {
    
    if (DS18B20_INITIALIZATION()) {

        DS18B20_Write_Byte(DS18B20_SKIP_ROM);
        DS18B20_Write_Byte(DS18B20_CONVERT);
        
        return 0;
    }
    else
        return -1;
}



int16_t DS18B20_Read() {
    
    if (DS18B20_INITIALIZATION()) {
        
        DS18B20_Write_Byte(DS18B20_SKIP_ROM);
        DS18B20_Write_Byte(DS18B20_READ_SCRATCHPAD);

        uint8_t tempL = DS18B20_Read_Byte();
        uint8_t tempH = DS18B20_Read_Byte();
        
        return (((tempH << 8) + tempL) * 0.625);
    } 
    else
        return -1;
}



/**
 * Convertit le port an10 en numérique
 *
 * @return valeur lue [0-1024]
 */
uint16_t convertAN10() {

    AD1CON1bits.ADON = 1;

    AD1CON1bits.SAMP = 1; //start sampling...

    __delay_us(10); // Ensure the correct sampling time has elapsed

    AD1CON1bits.SAMP = 0; //start converting

    while (!AD1CON1bits.DONE){}; // conversion done?

    AD1CON1bits.ADON = 0;
    
    //*valeur = ADC1BUF0;
    return ADC1BUF0; // yes then get ADC value
}

/**
 * 3000 = 0.75sec && 5000 = 1.25sec @2MHz
 */
void sleepDoze1Sec(uint8_t duree) {

    CLKDIVbits.DOZE = 0b111; // 1:128 (CPU-periph)
    CLKDIVbits.DOZEN = 1;
    if( duree == 1 ) {
        __delay_us(5859);       //0.75sec/128 (5859µs)
    } else if( duree == 2 ) {
        __delay_us(9765);        //1.25sec/128 (9765µs)
    } else if( duree == 3 )
        __delay_us(1562);       // 0.2s/128 (1562µs)
    CLKDIVbits.DOZEN = 0;
}