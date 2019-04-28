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

#ifndef MY_MACROS_PIC32MX440F256H_H    /* Guard against multiple inclusion */
#define MY_MACROS_PIC32MX440F256H_H 

/* DEFINES-MACROS*/
/* GENERAL */
#define VREF_MV 5000

/* TIMERS */
#define TIMER1_ENABLE() T1CONSET = _T1CON_ON_MASK
#define TIMER1_DISABLE() T1CONCLR = _T1CON_ON_MASK
#define TIMER1_IF_CLEAR() IFS0CLR = _IFS0_T1IF_MASK
#define TIMER1_RESET() TMR1 = 0;
#define TIMER1_IF IFS0bits.T1IF
#define TIMER1_SOURCE_INTERNAL() T1CONbits.TCS = 0 //TCS: Timer Clock Source Select bit
#define TIMER1_PRESCALE_1_1() T1CONbits.TCKPS = 0 // Timer Input Clock Prescale Select bits
#define TIMER1_PRESCALE_1_256() T1CONbits.TCKPS = 3 // Timer Input Clock Prescale Select bits
#define TIMER1_INTERR_EN() IEC0SET = _IEC0_T1IE_MASK
#define TIMER1_INTERR_DIS() IEC0CLR = _IEC0_T1IE_MASK
#define TIMER1_INTERR_PRIOR(x) IPC1bits.T1IP = x
#define TIMER1_INTERR_SUBPRIOR(x) IPC1bits.T1IS = x

/* INTERRUPTIONS */
#define GLOBAL_INTERR_EN()  __builtin_mtc0(12,0,(__builtin_mfc0(12,0) | 0x0001))  // Global interrupt enable
#define GLOBAL_INTERR_DIS()  __builtin_mtc0(12,0,(__builtin_mfc0(12,0) & 0xFFFE))  // Global interrupt disable
#define INTERR_MVEC_EN() INTCONbits.MVEC = 1

/* ADC */
#define ADC1_NUM_BITS 10
#define ADC1_SELECT_CHANNEL(x) AD1CHS = x << 16 // AD1CHS<16:19> controls which analog pin goes to the ADC
#define ADC1_ON() AD1CON1SET = _AD1CON1_ON_MASK
#define ADC1_OFF() AD1CON1CLR = _AD1CON1_ON_MASK
#define ADC1_IF_CLEAR() IFS1CLR = _IFS1_AD1IF_MASK
#define ADC1_AUTO_CONVERSION() AD1CON1SET = _AD1CON1_SSRC_MASK
#define ADC1_FORM_INT16() AD1CON1CLR = _AD1CON1_FORM_MASK
#define ADC1_VREF_TO_VDD() AD1CON2CLR = _AD1CON2_VCFG_MASK //AD1CON2<15:13>     
#define ADC1_USE_PERIPHERAL_CLOCK() AD1CON3CLR = _AD1CON3_ADRC_MASK
#define ADC1_USE_INTERNAL_RC_CLOCK() AD1CON3SET = _AD1CON3_ADRC_MASK
#define ADC1_TIME_SAMPLING(x) AD1CON3SET = (x << 8) & _AD1CON3_SAMC_MASK
#define ADC1_CLOCK_PRESCALE(x) AD1CON3SET = x & _AD1CON3_ADCS_MASK // To avoid nums larger than 512
#define ADC1_CKECK_DONE AD1CON1bits.DONE
#define ADC1_CLR_DONE() AD1CON1CLR = _AD1CON1_DONE_MASK
#define ADC1_SAMPLING AD1CON1bits.SAMP
#define ADC1_START_SAMPLING() AD1CON1bits.SAMP = 1
#define ADC1_CLR_DONE() AD1CON1CLR = _AD1CON1_DONE_MASK


#endif /* _PINGUINO_CONF_H */


