// Processor initialisation


#include "p30f6010A.h"

//configure oscillator
_FOSC(CSW_FSCM_OFF & HS3_PLL16); //26.667Mhz Fcy Tcy .0374995us

_FWDT(WDT_ON & WDTPSA_1 & WDTPSB_1); //watchdog timer on
_FBORPOR(MCLR_EN & PWMxH_ACT_HI & PWMxL_ACT_HI & PBOR_ON & BORV_42 & RST_IOPIN  );

void init(void)
{

//configure digital outputs
TRISGbits.TRISG0 = 0; 
TRISGbits.TRISG1 = 0; 
TRISGbits.TRISG2 = 0; 
TRISGbits.TRISG3 = 0; 
TRISGbits.TRISG6 = 0; 

//configure PWM
PTCONbits.PTMOD = 2;    //up down counting mode

PTPERbits.PTPER = 1777; //7.5Khz Fpwm

PWMCON1bits.PEN1L = 0;
PWMCON1bits.PEN2L = 0;
PWMCON1bits.PEN3L = 0;

PWMCON1bits.PEN1H = 0;
PWMCON1bits.PEN2H = 0;
PWMCON1bits.PEN3H = 0;

PWMCON2bits.IUE = 0;

//configure dead time
DTCON1bits.DTAPS = 3;
DTCON1bits.DTA = 10; //DT 2.99us//10

//fault input configuration
FLTBCONbits.FBEN1 = 0; //enable now
FLTBCONbits.FBEN2 = 0;
FLTBCONbits.FBEN3 = 0;

//timer1 configuration for sine sampling
PR1 = 267; //Period of 10us 
T1CONbits.TON = 0; //timer off

//timer2 configuration for program tracking
PR2 = 3000; //Period of 112us
T2CONbits.TON = 0; //timer off

//ADC configuration

ADCON1bits.SSRC = 7;
ADCON1bits.SIMSAM = 0;
ADCON1bits.FORM = 0;

ADCON1bits.FORM = 0x0; //q15 form

ADCON2= 0; //CH0 only

ADCON3bits.SAMC = 12; //sample time 12*Tad
ADCON3bits.ADCS = 4; //4//Tconv = 93.75ns

ADPCFG = 0x8013;
ADCSSL = 0;

ADCON1bits.ADON = 1; //adc on

//configure interrupts
IEC0bits.T1IE = 1; //enable

IPC0bits.T1IP = 0x7;

IEC2bits.PWMIE = 0; //disable pwm interrupt
}
