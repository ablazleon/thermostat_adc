/* ************************************************************************** */
/** Descriptive File Name
 @Date
    25-April-2018
  
  @Programmer
    Josué Pagán  
   
  @Company
    Universidad Politécnica de Madrid

  @File Name
    main.c

  @Summary
 Basic FSM reading analog data from a linear thermistor LM35 (0mv + 10mV/ºC).

  @Note: 
    josue: if we get this message: A heap is required, but has not been specified... 
    This is due to memory allocation in fsm_new. 
    See: http://www.microchip.com/forums/m737701.aspx
    and see "The heap" in https://os.mbed.com/handbook/Memory-Model
/* ************************************************************************** */

/* INLCUDES */
#include <xc.h>
#include <sys/attribs.h>
#include "myMacros_pic32mx440f256h.h"
#include "pinguinoConf.h"
#include "fsm.h"
#include "thermostat.h"

/* VARIABLES */
volatile int flags = 0x00;

enum fsm_state {
  HEATING,
  COOLING,
  SAVING_MODE,
};

/*
 * Maquina de estados: lista de transiciones
 * { EstadoOrigen, CondicionDeDisparo, EstadoFinal, AccionesSiTransicion }
 */
static fsm_trans_t thermostat_tt[] = {
  { COOLING, checkConfort, SAVING_MODE,  savingMode },
  { SAVING_MODE, checkCold, HEATING, heater_on},
  { HEATING,  checkConfort, SAVING_MODE, savingMode },
  { SAVING_MODE, checkHeat, COOLING, cooler_on },
  {-1, NULL, -1, NULL },
};

/* FUNCTIONS */
/* @Description: Interrupt Routine Service
 */
void __ISR(_TIMER_1_VECTOR, IPL5AUTO) timer1_ISR (void) {
    static int elapsedEvents = 0;   // Static: remain value in next calls to function
   
        double temp = volts2celsius(analogRead(ANALOG_CON2_A1_REG));    
        if (temp < THRESHOLD_LOW_TEMP){
            flags |= FLAG_START_HEATER;            
        } else if (temp > THRESHOLD_HIGH_TEMP) {
            flags |= FLAG_START_COOLING;       
        } else {
            flags |= FLAG_SAVING_MODE;
        }
    
    elapsedEvents = 0; // Restore counter
    
    TIMER1_IF_CLEAR();
}

void led_setup() {
    LED2_INIT(); // Configure PORT registers' for LED2
}

// Check functions
int checkHeat (fsm_t *this) {
    return (flags & FLAG_START_HEATER);
}

int checkCold (fsm_t *this) {
    return (flags & FLAG_START_COOLING);
}

int checkConfort (fsm_t *this){
    return (flags & FLAG_SAVING_MODE);
}

// Output functions
void savingMode (fsm_t *this) {
  //LED2_OFF();
  HEATER_OFF();
  COOLER_OFF();
  flags &= ~(FLAG_SAVING_MODE | FLAG_START_COOLING | FLAG_START_HEATER);
}

void heater_on (fsm_t *this) {
  //LED2_ON();
  HEATER_ON();
  flags &= ~(FLAG_START_HEATER  | FLAG_SAVING_MODE);
}

void cooler_on (fsm_t *this) {
  //LED2_ON();
  COOLER_ON();
  flags &= ~(FLAG_START_COOLING | FLAG_SAVING_MODE);
}


int analogRead(char analogPIN){
    ADC1_SELECT_CHANNEL(analogPIN);  // Select the input pins to be connected to the SHA.
  
    /* Ensure Persistent bits are cleared */
    ADC1_CLR_DONE();
    ADC1_IF_CLEAR();  
    
    ADC1_START_SAMPLING();  // Begin sampling

    while(ADC1_SAMPLING);  // Wait until acquisition is done
    while(!ADC1_CKECK_DONE);     // Wait until conversion done

    return ADC1BUF0;      // Result stored in ADC1BUF0
 }

void adcManualConfig(int prescale, int tadMultiplier){
    ADC1_OFF();    // disable ADC before configuration
    ADC1_AUTO_CONVERSION();       // Internal counter ends sampling and starts conversion (auto-convert), manual sample
    ADC1_FORM_INT16();    // (Default) Data output format int16 (only 10 bits available) 
    ADC1_VREF_TO_VDD();   // (Default) Set voltage reference to pins AVSS/AVDD
    ADC1_USE_PERIPHERAL_CLOCK(); // (Default) 
    ADC1_CLOCK_PRESCALE(prescale); 
    ADC1_TIME_SAMPLING(tadMultiplier);
    ADC1_ON(); // Enable ADC
}
void motorCooler_setup() {    
    COOLER_INIT(); // Configure PORT registers' for cooler
}

void sensor_setup(){
    ANALOG_CON2_A1_INIT(); // (Default) set RB2 (AN2) to analog. 0 as analog, 1 as digital
    ANALOG_CON2_A1_AS_INPUT(); // (Default) set RB2 as an input 
    //adcManualConfig(512, 15); // ADC clock = Peripheral clock / 512 --> TAD = 512*TPB
    //adcManualConfig?(?64?,? ?15?);    // Acquisition time = 15*TAD; Auto-sample time bits 
    adcManualConfig(0, 0);
}

void motorHeater_setup() {    
    HEATER_INIT(); // Configure PORT registers' for heater
}

void timer1_setup () {    
    // INT step 2
    TIMER1_PRESCALE_1_1(); // Prescaler 1
    PR1 = 15999; 
    TIMER1_SOURCE_INTERNAL(); // Internal peripheral clock
    TIMER1_ENABLE();  
    TIMER1_INTERR_PRIOR(5); // INT Step 3
    TIMER1_INTERR_SUBPRIOR(0); 
    TIMER1_RESET(); // INT Step 4
    TIMER1_INTERR_EN(); // INT Step 5   
}

double volts2celsius(int adcValue){
    double mvolts = (VREF_MV*adcValue)/((1<<ADC1_NUM_BITS)-1);
    double celsius = mvolts/10;
    return celsius;
}

int main() {
    fsm_t* fsm = fsm_new(thermostat_tt);
    
    motorCooler_setup(); // Configure Digital output for motor 1
    motorHeater_setup(); // Configure Digital output for motor 1
    //led_setup();
    timer1_setup();
    sensor_setup(); // Configure ADC
    savingMode(fsm);

    INTERR_MVEC_EN(); //Interrupt Controller configured for multi vectored mode
    GLOBAL_INTERR_EN(); // Enable global interrupt

    while(1) {
      fsm_fire(fsm);    
    }
    return 0;
}
