/* definition header for system registers and functions used in main*/
/* key program variables for changing setting */


#include "p30f6010A.h"

#define RL1_ON PORTGbits.RG0
#define RL2_ON PORTGbits.RG1
#define RL3_ON PORTGbits.RG2
#define RL4_ON PORTGbits.RG3
#define RL5_ON PORTGbits.RG6

#define PWM1 PDC1 //R PWM
#define PWM2 PDC2 //Y PWM
#define PWM3 PDC3 //B PWM

#define T1us_Flag IFS0bits.T1IF 
#define T2us_Flag IFS0bits.T2IF 

#define PWM_PERIOD PTPER 
#define Converstion_Done ADCON1bits.DONE
#define Start_Converstion ADCON1bits.SAMP

#define CH0_MULA ADCHSbits.CH0SA
#define CH0_MULB ADCHSbits.CH0SB

#define PWMenable PTCONbits.PTEN

#define T1ON T1CONbits.TON

#define ADC_ON ADCON1bits.ADON

#define FAULT_ENABLE FLTBCON

#define SET PWMCON1

//user control below

//************************************************************//
#define PI 3.1415926536                     //value of pi
#define samples 60.0                        //number of samples in sine wave
#define SineResolution 360.0/samples        //resolution of sine wave
//************************************************************//

//************************************************************//
//counters for program tracking
/* sine sampling             //320us - 2778us */     //priority1
#define DCLinkCount 420      //3000us                //priority2
#define PID_I_count 452      //3420us                //priority3
#define FrequencyUpdate 600  //6000us                //priority4
#define softCount 900        //9000us                //priority5
//************************************************************//

//************************************************************//
#define VDCLink_Trip 895 //@175Volts //dc link over voltage
#define VDCBalance 220 //@50Volts //dc link unbalance voltage
#define DCLinkMin 100 //minimum dc link vltage//20 //dc link under voltage
//************************************************************//

#define SPEEDREF 1544 //max 2000      //set speed ref of the motor
#define SPEEDREF_MIN 220 //min 200    //initial speed ref of the motor

#define CONST_K4 0xef45                //motor constant// see theory.txt file for details


//global function definitions///////////////////////////////////

int adc(char);                  //adc function
void delay(unsigned int);       //general purpose delay
void init(void);                //software initialization
////////////////////////////////////////////////////////////////
