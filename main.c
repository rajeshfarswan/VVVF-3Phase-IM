//********************************************************************//
// AC LAB KIT 3PHASE SINE PWM CONFIGURATION PROGRAM for open loop V/F //
//********************************************************************//

//
#include "p30f6010A.h"


#include "main.h"    // Include file containing processor registors definitions and function definitions
#include "math.h"    // MPLAB math library
#include "asmMATH.h" // Include definition of math functions 

//
char fault_flag = 0; 
int i;
//
int Sine_sample = 0;
int PID_Isample = 0; 
int softStart = 0; 
int DCLinkSample = 0;
unsigned int FrequencyCount = 0;
unsigned int Frequency = 0;

unsigned int VDC1 = 0;
unsigned int VDC2 = 0;
unsigned int VDC = 0;
unsigned int Vavg = 0;

unsigned int Modulation = 0;
int F_Update = 0;
int SPEED_MIN = 0;
int SAMPLES = 0;
int FAULT = 0;
int SOFT = 0;
int SET_SPEED = 0;
int DCLINK = 0;
int UNBALANCE = 0;
int DCLINK_MAX = 0;
unsigned int SpeedRef = 0;
unsigned int CONSTANT_K4 = 0;

unsigned int *p_avgARRAY; //pointer to averaging array
//
unsigned int AVG[4] = {2046,2046,2046,2046}; //array for moving average

int SINE_A[(int)samples] = {0}; //three phase sine table
int SINE_B[(int)samples] = {0};
int SINE_C[(int)samples] = {0};
int Data_pointer; //pointer to sine table
//
void sinetable(void); //three phase sine table generation function
//

int main(void)
{
init();        // call processor initialisation code
PWMenable = 0; //initialise PWM control registers
delay(50);     //delay 100ms

//precharging init
RL1_ON = 1;  //precharging enable
delay(1500); //delay 1500ms
RL1_ON = 0;  //precharging disable
delay(30);   //delay 30ms
RL2_ON = 1;  //bypass precharging
delay(30);   //delay 30ms
//precharging init ends

//
sinetable();               //program sine table
i = 0;                     //reset i
Data_pointer = 0;          //reset data pointer
//
F_Update = FrequencyUpdate; //set sine frequency counter (from main.h)
SPEED_MIN = SPEEDREF_MIN;   //set initial speed ref (from main.h)
SAMPLES = samples - 0x0001; //set number of sample points for sine wave (from main.h)
FAULT = PID_I_count;        //set PI sampling frequency (from main.h)
SOFT = softCount;           //set soft start delay counter (from main.h)
SET_SPEED = SPEEDREF;       //set open loop speed  (from main.h)
DCLINK = DCLinkCount;       //set dc link function sampling  rate (from main.h)
UNBALANCE = VDCBalance;     //set unbalance value in dc link (from main.h)
DCLINK_MAX = VDCLink_Trip;  //set max value of dc link protection (from main.h)

CONSTANT_K4 = CONST_K4;     //K4 constant to convert speed ref to frequency (theory.txt)

p_avgARRAY = AVG;           //assign pointer to averaging array
//

//initiates startup////////////////////////////////////////////////////// 
SET = 0;
PWMenable = 1;  //enable and initialise inverter pwm
PWM1 = 0x6f1;
PWM2 = 0x6f1;
PWM3 = 0x6f1;
SET = 0x0077;  //all three switces are enabled
//

//
T1ON = 1;                 //enable timer 1 for program tracking
FAULT_ENABLE = 0x000f;    //enable fault
ClrWdt();                 //clear watchdog timer
//
SpeedRef = SPEEDREF_MIN;  //set initial speed ref to minimum
Vavg = 2046;              //set initial avg value

Modulation = asmINT_DIVQ(SpeedRef, Vavg);         //set initial modulation index value = speedref/dc link

Frequency = asmINT_USDIV(CONSTANT_K4, SpeedRef);  //set initial frequency of inverter = K4/speedref
//


  while(1) 
	{ 

                                             
if(FrequencyCount > F_Update) //inverter frequency update routine
{
//calculate ferquency required
VDC1 = adc(3);
VDC2 = adc(2);
VDC = VDC1+VDC2; //total dc link value

////sample dc link values in array
if(i>3) i = 0;

AVG[i] = VDC;
i++;

//
Vavg = asmINT_AVG(p_avgARRAY,4,2);               //moving average of elements in array
//
Modulation = asmINT_DIVQ(SpeedRef, Vavg);        //calculate desired modulation index = speed_ref/dc_link_value         
//
Frequency = asmINT_USDIV(CONSTANT_K4, SpeedRef); //determine inverter frequency = constant/speed_ref  
}

//Inverter duty cycle program
if(Sine_sample > Frequency)
   {

    if(Data_pointer > SAMPLES) //reset array data pointer if array ends
    {
     Data_pointer = 0; //reset pointer
    }

asm("disi #0x3FFF"); //set duty cycles
{
PWM1 = asmINT_MPQ(Modulation, SINE_A[Data_pointer]) + 0x6f1; //phase a = (modulation_index*sine_Avalue) + period_value
PWM2 = asmINT_MPQ(Modulation, SINE_B[Data_pointer]) + 0x6f1; //phase b = (modulation_index*sine_Bvalue) + period_value
PWM3 = asmINT_MPQ(Modulation, SINE_C[Data_pointer]) + 0x6f1; //phase c = (modulation_index*sine_Cvalue) + period_value
}
asm("disi #0x0000");

Data_pointer++;

Sine_sample = 0;

    } //sine sampling end


//If fault flag is set disable PWM and open relays
if(PID_Isample > FAULT)
{

if(fault_flag) 
    {
     PWMenable = 0; //pwm off
     RL1_ON = 0;    //relays off
     RL2_ON = 0;
     }
PID_Isample = 0;

} //fault polling ends

if(softStart > SOFT) 
   {

      SpeedRef++;                 //set open loop speed ref
      if(SpeedRef >= SET_SPEED) 
      {
       SpeedRef = SET_SPEED;      //max speed ref set point
    
       } 
      
   softStart = 0;
   } 
//soft start ends

//dc link fault monitor
if(DCLinkSample > DCLINK)
    {

   if((VDC1 > DCLINK_MAX) || (VDC2 > DCLINK_MAX)) //if dc link fault
   fault_flag = 1; //set fault

   if(VDC1 > VDC2)  //dc-link unalance trip fault
    {
      if((VDC1-VDC2) > UNBALANCE)
        fault_flag = 1; //set fault
    }

   else 
       {
      if((VDC2 - VDC1) > UNBALANCE) 
        fault_flag = 1; //set fault
       }

   DCLinkSample = 0;

   } //DCLink Status monitor end

ClrWdt();
    } //while end

ClrWdt();
} //main end

//T1 interrupt for program tracking///////////////////////////////////////
void _ISRFAST __attribute__((interrupt, no_auto_psv)) _T1Interrupt(void)
{

PID_Isample++;
softStart++;
Sine_sample++;
DCLinkSample++;
FrequencyCount++;

T1us_Flag = 0;

   } //T1 interupt end
///////////////////////////////////////////////////////////////////////



//write sine data in array //value in between +-pwm_period
void sinetable()
{
asm("disi #0x3FFF");
    for(i=0;i<=(samples-1);i++) //a phase sine
    {
    SINE_A[i] = (sin(PI*SineResolution*i/180.0)+0.0)*PWM_PERIOD;

    ClrWdt();
    }

    for(i=0;i<=(samples-1);i++) //b phase sine
    {
    SINE_B[i] = (sin((PI/180.0)*(SineResolution*i-120.0))+0.0)*PWM_PERIOD;

    ClrWdt();
    }

     for(i=0;i<=(samples-1);i++) //c phase sine
    {
    SINE_C[i] = (sin((PI/180.0)*(SineResolution*i-240.0))+0.0)*PWM_PERIOD;

    ClrWdt();
    }
asm("disi #0x0000");
} //writing sine data ends


