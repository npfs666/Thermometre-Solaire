/**
 * SharpLCD.h
 *
 * Drivers pour un écran sharp 96*96 pixel
 * SHARP Memory Display Breakout - 1.3" 96x96 Silver Monochrome
 *
 * Développé sur MPLABX pour les compilateurs XC
 *
 * Auteur : Brendan Jaouen
 *
 * Date : Juin 2017
 */


/*
 Commandes de l'écran SHARP
 */
#define SHARPMEM_BIT_WRITECMD   0x80
#define SHARPMEM_BIT_VCOM       0x40
#define SHARPMEM_BIT_CLEAR      0x20

/**
 * Ports nécéssaires au fonctionnement
 *
 * PIC      SHARP
 * SPI1CLK  CLK
 * SPI1DO   DI
 * CS       CS
 */

void SharpClearDisplay();
uint8_t switchVCOM();
void SharpStaticMode();
void setPixel(uint8_t x, uint8_t y, uint8_t value);
void drawHorLine(uint8_t x1, uint8_t x2, uint8_t y);
void drawVertLine(uint8_t x, uint8_t y1, uint8_t y2);
void SharpRefresh();