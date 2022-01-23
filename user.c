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

#include "user.h"            /* variables/params used by user.c */
#include "system.h"
#include "SharpLCD.h"

/******************************************************************************/
/* User Functions                                                             */
/******************************************************************************/



void InitApp(void) {

    ANSELA = 0;
    ANSELB = 0;
    ANSELAbits.ANSA0 = 1;  // Photodetecteur
    ANSA = 0;
    ANSB = 0;
    ANSAbits.ANSA0 = 1;    // Photodetecteur

    TRISA = 0;
    PORTA = 0;
    TRISAbits.TRISA4 = 1;   // Quartz 32 768Hz
    
    TRISB = 0;
    PORTB = 0;
    TRISBbits.TRISB4 = 1;   // Quartz 32 768Hz
    TRISAbits.TRISA0 = 1;  // Photodetecteur
    
    initRTCC();
    initSPI();
    initADC();
    SHARP_PWR = 1;
    SharpClearDisplay();
    SharpStaticMode();
}



void setRTCWREN(void) {

    //Débloquage du bit RTCWREN pour autoriser la modification du registre RTCC
    asm volatile ("push w7");
    asm volatile ("push w8");
    asm volatile ("disi #5");
    asm volatile ("mov #0x55, w7");
    asm volatile ("mov w7, _NVMKEY");
    asm volatile ("mov #0xAA, w8");
    asm volatile ("mov w8, _NVMKEY");
    asm volatile ("bset _RCFGCAL, #13"); //set the RTCWREN bit
    asm volatile ("pop w8");
    asm volatile ("pop w7");
}



void initRTCC(void) {

    setRTCWREN();

    // Configuration du RTCC
    RTCPWCbits.RTCCLK = 0b00;   // external secondary oscillator  (SOSC)
    ALCFGRPTbits.CHIME = 1;     // Décompte à l'infini
    ALCFGRPTbits.AMASK = 0b0001;// Every minute (0011)

    // Activation de l'IT du RTCC
    IEC3bits.RTCIE = 1;         //Real-Time Clock and Calendar Interrupt Enable bit
    //IPC15bits.RTCIP = 0b010;    // priorité 2
    ALCFGRPTbits.ALRMEN = 1;    // enable alarm
    IFS3bits.RTCIF = 0;

    // Reset alarm values
    
    uint8_t day=1, heures = 18, minutes = 58, secondes = 0;
    RCFGCALbits.RTCPTR=0b00;
    RTCVAL = ((minutes/10)<<12) + ((minutes%10)<<8) + ((secondes/10)<<4)+ (secondes%10);
    RCFGCALbits.RTCPTR=0b01;
    RTCVAL = (day<<8) + ((heures/10)<<4) + (heures%10);
    RCFGCALbits.RTCPTR=0b10;
    RTCVAL = 0;
    RCFGCALbits.RTCPTR=0b11;
    RTCVAL = 0;

    // Lancement du RTCC
    RCFGCALbits.RTCEN = 1;
}



void startRTC(void) {
    setRTCWREN();
    RCFGCALbits.RTCEN = 1;
}



void stopRTC(void) {
    setRTCWREN();
    RCFGCALbits.RTCEN = 0;
}



void initSPI(void) {
    
    SPI1CON1bits.MODE16 = 0;    // mot de 8bits
    SPI1CON1bits.MSTEN = 1;     // Master mode
    SPI1CON1bits.SPRE = 0b111;  // secondary prescaler 1:1
    SPI1CON1bits.PPRE = 0b10;   // primary prescaler 1:4 (SPI @ 4mhz/4 = 1MHz)
    SPI1CON1bits.CKE = 1;        
    SPI1CON1bits.CKP = 0;
    
    SPI1STATbits.SPIROV = 0;    // clear flag
    SPI1STATbits.SPIEN = 1;     // enable spi
}



/**
 * Registres ADC, conversion manuelle
 */
void initADC(void) {

    AD1CON1 = 0x0000;       // SAMP bit = 0 ends sampling and starts converting
    AD1CON1bits.MODE12 = 0; //10bits mode
    AD1CON2bits.PVCFG = 0b00; // Vref+=aVdd

    AD1CHS = 0x0000;  // Connect AN0 as S/H+ input
    AD1CHSbits.CH0SA = 0b00000; // RA1 = AN1
    AD1CSSL = 0;
    AD1CON3 = 0x0000; // Manual Sample, Tad = Tcy
    AD1CON2 = 0;
}