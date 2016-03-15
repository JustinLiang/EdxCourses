// DAC.c
// Runs on LM4F120 or TM4C123, 
// edX lab 13 
// Implementation of the 4-bit digital to analog converter
// Daniel Valvano, Jonathan Valvano
// December 29, 2014
// Port B bits 3-0 have the 4-bit DAC

#include "DAC.h"
#include "..//tm4c123gh6pm.h"

// **************DAC_Init*********************
// Initialize 4-bit DAC 
// Input: none
// Output: none
void DAC_Init(void){
	unsigned long delay;
	SYSCTL_RCGC2_R |= 0x02; // Activate clock for port B
	delay = SYSCTL_RCGC2_R;
	GPIO_PORTB_DATA_R = 0x400053FC;
	GPIO_PORTB_LOCK_R = 0x4C4F434B; // Unlock GPIO port B
	GPIO_PORTB_CR_R = 0x3F; // Allow changes to PB0 to PB5
	GPIO_PORTB_AMSEL_R = 0x00; // Disable analog for PB
	GPIO_PORTB_PCTL_R = 0x00; // Disable alternate functions
	GPIO_PORTB_DIR_R = 0x3F; // PB0 to PB5 are outputs
	GPIO_PORTB_AFSEL_R = 0x00; // Disable alternate functions
	GPIO_PORTB_DEN_R = 0x3F; // Digital enable for PB0 to PB5
	GPIO_PORTB_DR8R_R = 0x3F; // Allow for 8 mA drive on the selected pins
	
}


// **************DAC_Out*********************
// output to DAC
// Input: 4-bit data, 0 to 15 
// Output: none
void DAC_Out(unsigned long data){
  GPIO_PORTB_DATA_R = data;
}
