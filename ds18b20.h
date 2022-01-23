/* 
 * File:   ds18b20.h
 * Author: Brendan
 *
 * Created on 12 septembre 2014, 21:22
 */



//Edit those value
# define DS18B20_Read_PIN PORTBbits.RB7
# define DS18B20_Write_PIN LATBbits.LATB7
# define DS18B20_Tris_PIN TRISBbits.TRISB7
//Don't tuch
# define Input 1
# define Output 0
//http://www.maximintegrated.com/app-notes/index.mvp/id/126
# define DS18B20_PARAM_A 6
# define DS18B20_PARAM_B 64
# define DS18B20_PARAM_C 60
# define DS18B20_PARAM_D 10
# define DS18B20_PARAM_E 9
# define DS18B20_PARAM_F 55
# define DS18B20_PARAM_G 0
# define DS18B20_PARAM_H 480
# define DS18B20_PARAM_I 70
# define DS18B20_PARAM_J 410
//ROM Command
# define DS18B20_SEARCH_ROM 0xF0
# define DS18B20_READ_ROM 0x33
# define DS18B20_MATCH_ROM 0x55
# define DS18B20_SKIP_ROM 0xCC
# define DS18B20_ALARM_SEARCH 0xEC
//Function Commands
# define DS18B20_CONVERT 0x44
# define DS18B20_WRITE_SCRATCHPAD 0x4E
# define DS18B20_READ_SCRATCHPAD 0xBE
# define DS18B20_COPY_SCRATCHPAD 0x48
# define DS18B20_RECALLE2 0xB8
# define DS18B20_READ_POWER_SUPPLY 0xB4
//Conf register
#define DS18B20_9_Bit_Mode 0b00011111
#define DS18B20_10_Bit_Mode 0b00111111
#define DS18B20_11_Bit_Mode 0b01011111
#define DS18B20_12_Bit_Mode 0b01111111

void DS_18B20_init(void);
void DS18B20_Write_bit(uint8_t w);
uint8_t DS18B20_Read_bit(void);
uint8_t DS18B20_INITIALIZATION(void);
void DS18B20_Write_Byte(uint8_t o);
uint8_t DS18B20_Read_Byte();
void DS18B20_to_string(uint8_t H, uint8_t L, char* result);




