/* ************************************************************************** */
/** Descriptive File Name
 @Date
    09-April-2018
  
  @Programmer
    Josué Pagán  
 
  @Company
    Universidad Politécnica de Madrid
 */
/* ************************************************************************** */

#ifndef _PINGUINO_CONF_H    /* Guard against multiple inclusion */
#define _PINGUINO_CONF_H 

/* DEFINES-MACROS*/
#define LED2_TOGGLE() PORTDINV = 0x0002
#define LED2_OFF() PORTDbits.RD1 = 0
#define LED2_ON() PORTDbits.RD1 = 1
#define LED2_INIT() TRISDbits.TRISD1 = 0

#define BUTTON_INIT() TRISDbits.TRISD0 = 1
#define BUTTON_PUSHED  IFS0bits.INT0IF
#define BUTTON_IF_CLEAR() IFS0CLR = _IFS0_INT0IF_MASK // IFS0bits.INT0IF = 0
#define BUTTON_INTERR_EN() IEC0SET = _IEC0_INT0IE_MASK // IEC0bits.INT0IE = 1
#define BUTTON_INTERR_DIS() IEC0CLR = _IEC0_INT0IE_MASK // IEC0bits.INT0IE = 0
#define BUTTON_EDGE_RISING()  INTCONbits.INT0EP = 1
#define BUTTON_EDGE_FALLING() INTCONbits.INT0EP = 0
#define BUTTON_INTERR_PRIOR(x) IPC0bits.INT0IP = x
#define BUTTON_INTERR_SUBPRIOR(x) IPC0bits.INT0IS = x

#define ANALOG_CON2_A1_REG 2
#define ANALOG_CON2_A1_INIT() AD1PCFGCLR = _AD1PCFG_PCFG2_MASK
#define ANALOG_CON2_A1_AS_INPUT() TRISBSET = _TRISB2


#endif /* _PINGUINO_CONF_H */


