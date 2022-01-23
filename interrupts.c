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

#include <stdint.h>        /* Includes uint16_t definition */
#include <stdbool.h>       /* Includes true/false definition */

#include "system.h"
#include "user.h"
#include "SharpLCD.h"
#include "font.h"
#include <libpic30.h>
#include <stdio.h>

/******************************************************************************/
/* Interrupt Routines                                                         */
/******************************************************************************/

int8_t heures, minutes, secondes, day, jour, mois, annee;
uint8_t last_conv = 1;
uint16_t ii=0;

/**
 * Gestion de l'IT du RTCC
 */
void __attribute__((interrupt,no_auto_psv)) _RTCCInterrupt(void) {
    
    LIGHTSENS_PWR = 1;
    //LED = 1;
    
    uint16_t lightSens = convertAN10();
    
    if( lightSens > 350 ) {
        
        // on arrive dans le noir
        if( last_conv != 60) {
            
            SharpClearDisplay();
            last_conv = 60;
            SHARP_PWR = 0;
            DS18B20_PWR = 0;
            DCDC_PWR = 0;
        }
        
        LIGHTSENS_PWR = 0;
        _RTCIF = 0;
        Sleep();
        return;
        
        /*SharpClearDisplay();
        last_conv = 60;
        SHARP_PWR = 0;
        DS18B20_PWR = 0;
        DCDC_PWR = 0;
         LIGHTSENS_PWR = 0;
        _RTCIF = 0;
        Sleep();
        return;*/
    }
    LIGHTSENS_PWR = 0;
    
    
    getRTCCMinSec(&minutes, &secondes);
    
    /*SHARP_PWR = 1;
    SharpClearDisplay();
    char tmp[3];
    sprintf(tmp, "%d.", lightSens);
    DrawString(tmp, 0, 0, 3);
    SharpRefresh();*/
    
    // Seconde 1, lancement d'une conversion de température
    if( (last_conv == 60) || (secondes == 1)  ) {
        
        //LED = 1;
        DCDC_PWR = 1;
        DS18B20_PWR = 1;
        last_conv++;
        __delay_ms(2);
        
        DS18B20_Convert();
        
    }
    // Seconde 2, lecture et affichage de la mesure
    else if( (last_conv == 61) || (secondes == 2) ) {
        
        //LED = 1;
        char tmp[3];
        
        SHARP_PWR = 1;
        int16_t temp = DS18B20_Read();
        //temp = 0xFFF8;
        DS18B20_PWR = 0;
        DCDC_PWR = 0;
        
        SharpClearDisplay();

        if( temp < 0 ) {
            
            temp *= -1;
            
            sprintf(tmp, "-");
            DrawString(tmp, 2, 14, 2);
        }
        sprintf(tmp, "%02d.", temp/10);
        DrawString(tmp, 15, 0, 6);
        
        sprintf(tmp, "%dC", temp%10);
        DrawString(tmp, 15, 50, 6);

        SharpRefresh();
        last_conv = secondes;
    }
    // Sinon toggle vcom
    else {
        //SHARP_PWR = 1;
        SharpStaticMode();
    }
    
    _RTCIF = 0;
    Sleep();
}
