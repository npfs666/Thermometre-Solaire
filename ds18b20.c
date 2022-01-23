/* 
 * File:   ds18b20.c
 * Author: Brendan
 *
 * Created on 12 septembre 2014, 21:24
 */

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

#include <stdint.h>
#include <stdio.h>

#include "ds18b20.h"
#include "system.h"

#include <libpic30.h>





void DS_18B20_init(void) {
    DS18B20_Tris_PIN = Input;
}

void DS18B20_Write_bit(uint8_t w) {

    if (w == 1) {
        DS18B20_Tris_PIN = Output;
        DS18B20_Write_PIN = 0;
        __delay_us(DS18B20_PARAM_A);
        DS18B20_Tris_PIN = Input;
        __delay_us(DS18B20_PARAM_B);
    } else {
        DS18B20_Tris_PIN = Output;
        DS18B20_Write_PIN = 0;
        __delay_us(DS18B20_PARAM_C);
        DS18B20_Tris_PIN = Input;
        __delay_us(DS18B20_PARAM_D);
    }
    
}

uint8_t DS18B20_Read_bit(void) {
    uint8_t read_value;
    DS18B20_Tris_PIN = Output;
    DS18B20_Write_PIN = 0;
    __delay_us(DS18B20_PARAM_A);
    DS18B20_Tris_PIN = Input;
    __delay_us(DS18B20_PARAM_E);
    read_value = DS18B20_Read_PIN;
    __delay_us(DS18B20_PARAM_F);
    return read_value;
}

uint8_t DS18B20_INITIALIZATION(void) {
    uint8_t read_value;
    DS18B20_Tris_PIN = Input;
    __delay_us(DS18B20_PARAM_G);
    DS18B20_Tris_PIN = Output;
    DS18B20_Write_PIN = 0;
    __delay_us(DS18B20_PARAM_H);
    DS18B20_Tris_PIN = Input;
    __delay_us(DS18B20_PARAM_I);
    read_value = DS18B20_Read_PIN;
    __delay_us(DS18B20_PARAM_J);
    return !read_value;
}

void DS18B20_Write_Byte(uint8_t o) {
    uint8_t i = 0, bits = 0;
    for (i = 0; i < 8; ++i) {
        bits = (o >> i) & 0x01;
        DS18B20_Write_bit(bits);
    }
}

uint8_t DS18B20_Read_Byte() {
    uint8_t bits = 0, byte = 0, i;
    for (i = 0; i < 8; ++i) {
        bits = DS18B20_Read_bit();
        byte = byte | (bits << i);
    }
    return byte;
}
