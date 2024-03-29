/* ************************************************************************** */
/** Descriptive File Name

  @Date
    24-April-2018
  
  @Programmer
    Josu� Pag�n  
   
  @Company
    Universidad Polit�cnica de Madrid

  @File Name
    thermostat.h

  @Description
    Define prototypes of functions and other elements.
 */
/* ************************************************************************** */

#ifndef _THERMOSTAT_H    /* Guard against multiple inclusion */
#define _THERMOSTAT_H

/* DEFINES */
#define FLAG_START_HEATER 0x01
#define FLAG_START_COOLING 0x02
#define FLAG_SAVING_MODE 0x04

#define THRESHOLD_HIGH_TEMP 25.0 // MAX �C
#define THRESHOLD_LOW_TEMP 22.0 // MIN �C
#define SAMPLING_PERIOD_SEC 2 // ms

#define HEATER_OFF() PORTDbits.RD10 = 0
#define HEATER_ON() PORTDbits.RD10 = 1
#define HEATER_INIT() TRISDbits.TRISD10 = 0
#define COOLER_OFF() PORTDbits.RD11 = 0
#define COOLER_ON() PORTDbits.RD11 = 1
#define COOLER_INIT() TRISDbits.TRISD11 = 0

/* PROTOTYPES FUNCTIONS */
/* Configuration functions */
void led_setup(void);
void timer1_setup(void);
void sensor_setup(void);
void motorCooler_setup(void);
void motorHeater_setup(void);

/* Check functions*/
int checkHeat (fsm_t *this);
int checkCold (fsm_t *this);
int checkConfort (fsm_t *this);

/* Actuation functions */
void savingMode (fsm_t *this);
void heater_on (fsm_t *this);
void cooler_on (fsm_t *this);

/* Others */
void adcManualConfig(int prescale, int tadMultiplier);
int analogRead(char analogPIN);
double volts2celsius(int adcValue);

#endif /* _THERMOSTAT_H */