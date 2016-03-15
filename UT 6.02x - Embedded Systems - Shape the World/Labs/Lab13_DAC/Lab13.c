// Lab13.c
// Runs on LM4F120 or TM4C123
// Use SysTick interrupts to implement a 4-key digital piano
// edX Lab 13 
// Daniel Valvano, Jonathan Valvano
// December 29, 2014
// Port B bits 3-0 have the 4-bit DAC
// Port E bits 3-0 have 4 piano keys

#include "..//tm4c123gh6pm.h"
#include "Sound.h"
#include "Piano.h"
#include "TExaS.h"
#include "DAC.h"

#define G0    3189   // 784 Hz
#define E0   3792   // 659.3 Hz
#define D0   4257   // 587.3 Hz
#define C0  4778   // 523.3 Hz


// Define the keys
enum key {
	NONE = 0,
	A,
	B,
	C,
	D,
	E,
	F,
	G,
};



unsigned char n=0;
/*const unsigned char wave[32] = {
  8,9,11,12,13,14,14,15,15,15,14,
  14,13,12,11,9,8,7,5,4,3,2,
  2,1,1,1,2,2,3,4,5,7};*/
//const unsigned char wave[32] = {5,5,4,1,1,3,8,11,11,9,4,
//2,5,11,15,13,9,7,5,5,6,8,8,7,4,3,3,3,3,4,5,5};  
int delayValue=25;

// basic functions defined at end of startup.s
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void delay(unsigned long msec);
int main(void){ // Real Lab13 
	// for the real board grader to work 
	// you must connect PD3 to your DAC output
  TExaS_Init(SW_PIN_PE3210, DAC_PIN_PB3210,ScopeOn); // activate grader and set system clock to 80 MHz
// PortE used for piano keys, PortB used for DAC        
  Sound_Init(); // initialize SysTick timer and DAC
  Piano_Init();
  EnableInterrupts();  // enable after all initialization are done
  while(1){         
		
		/*DAC_Out(wave[n]);     // your code to output to the DAC
    delay(1000); // wait 1s (later change this to 1ms)
    n = (n+1)&0x1F;*/
		
		switch(Piano_In()) {
			case C:
				delay(delayValue);
				//EnableInterrupts();
				//Sound_Tone(523.251);
				Sound_Tone(C0);
				break;
			case D:
				//EnableInterrupts();
				delay(delayValue);
				Sound_Tone(D0);
				break;
			case E:
				//EnableInterrupts();
				delay(delayValue);
				Sound_Tone(E0);
				break;
			case G:
				//EnableInterrupts();
				delay(delayValue);
				Sound_Tone(G0);
				break;
			default:
				delay(delayValue);
				Sound_Off();
				//DisableInterrupts();
				break;
		}
	
	}
            
}

// Inputs: Number of msec to delay
// Outputs: None
void delay(unsigned long msec){ 
  unsigned long count;
  while(msec > 0 ) {  // repeat while there are still delay
    count = 16000;    // about 1ms
    while (count > 0) { 
      count--;
    } // This while loop takes approximately 3 cycles
    msec--;
  }
}


