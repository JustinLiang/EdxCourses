// Sound.c
// Runs on LM4F120 or TM4C123, 
// edX lab 13 
// Use the SysTick timer to request interrupts at a particular period.
// Daniel Valvano, Jonathan Valvano
// December 29, 2014
// This routine calls the 4-bit DAC

#include "Sound.h"
#include "DAC.h"
#include "..//tm4c123gh6pm.h"

#define ON 1
#define OFF 0

// Sine Wave
//int N = 32;
//const unsigned char Wave[32] = {
//  8,9,11,12,13,14,14,15,15,15,14,
//  14,13,12,11,9,8,7,5,4,3,2,
//  2,1,1,1,2,2,3,4,5,7};

// Trumpet																		
//int N = 32;
//const unsigned char Wave[32] = {10,11,11,12,10,8,3,1,8,15,15,11,
//	10,10,11,10,10,10,10,10,10,10,10,11,11,11,11,11,11,10,10,10};

//Guitar																		
//int N = 32;
const unsigned char Wave[32] = {5,5,4,1,1,3,8,11,11,9,4,
2,5,11,15,13,9,7,5,5,6,8,8,7,4,3,3,3,3,4,5,5};  // man wtf freq is 2 times larger than what it should be 

//Flute
//int N = 64;
//const unsigned char Wave[64] = {  6,7,9,9,10,11,12,13,13,14,15,
//  15,15,15,15,14,13,13,12,11,10,9,8,7,7,6,6,5,5,5,5,4,
//  4,4,4,4,4,3,3,3,3,3,2,2,1,1,1,1,0,0,0,0,0,0,1,1,1,2,2,3,3,4,4,5};  

unsigned char Index=0;           // Index varies from 0 to 15
unsigned char flag=0;

// **************Sound_Init*********************
// Initialize Systick periodic interrupts
// Also calls DAC_Init() to initialize DAC
// Input: none
// Output: none
void Sound_Init(void){
  
	DAC_Init(); // Port B is DAC
	Index = 0;
	NVIC_ST_CTRL_R = 0; // disable SysTick during setup
	NVIC_ST_RELOAD_R = 0x00FFFFFF; // Setup reload to be max possible value
	NVIC_ST_CURRENT_R = 0; // any write to current clears it
	NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x20000000;
	NVIC_ST_CTRL_R = 0x0007; // Enable, core clock and enterrupts
	
}

// **************Sound_Tone*********************
// Change Systick periodic interrupts to start sound output
// Input: interrupt period
//           Units of period are 12.5ns
//           Maximum is 2^24-1
//           Minimum is determined by length of ISR
// Output: none
void Sound_Tone(unsigned long period){ // note period is actually a frequency
// this routine sets the RELOAD and starts SysTick
	//period = 80000000/(period*N)+1;
	NVIC_ST_RELOAD_R = period;
	flag = ON;
}


// **************Sound_Off*********************
// stop outputing to DAC
// Output: none
void Sound_Off(void){
 // this routine stops the sound output
	//NVIC_ST_RELOAD_R = 0;
	DAC_Out(0);
	flag = OFF;

}


// Interrupt service routine
// Executed every 12.5ns*(period)
void SysTick_Handler(void){
	if(flag == ON) {
		Index = (Index+1)&0x1F;
		DAC_Out(Wave[Index]);
	}
}
