/******************************************************************************/
/* System Level #define Macros                                                */
/******************************************************************************/

/* Microcontroller MIPs (FCY) */
#define SYS_FREQ        8000000L
#define FCY             SYS_FREQ/2

// Ports pour l'alim
#define DCDC_PWR       LATBbits.LATB12

// Ports du photodétecteur
#define LIGHTSENS_PWR   LATBbits.LATB15
#define LIGHTSENS_ANALOG PORTAbits.RA0

// Ports du SHARP
#define SHARP_PWR       LATAbits.LATA6
#define SHARP_CLK       LATBbits.LATB11
#define SHARP_DI        LATBbits.LATB13
#define SHARP_CS        LATBbits.LATB14

// Ports DS18B20
#define DS18B20_PWR     LATBbits.LATB8
#define DS18B20_COM     LATBbits.LATB7
    


/******************************************************************************/
/* System Function Prototypes                                                 */
/******************************************************************************/

/* Custom oscillator configuration funtions, reset source evaluation
functions, and other non-peripheral microcontroller initialization functions
go here. */

void ConfigureOscillator(void); /* Handles clock switching/osc initialization */
int16_t readEEP(const uint16_t adress);
void writeEEP(uint16_t address, int16_t data);
void getRTCCTimeData(int8_t *wDay, int8_t *hour, int8_t *min, int8_t *sec);
int16_t DS18B20_Convert();
int16_t DS18B20_Read();
void getRTCCMinSec(int8_t *min, int8_t *sec);
uint16_t convertAN10();