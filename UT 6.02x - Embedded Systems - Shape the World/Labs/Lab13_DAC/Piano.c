// Piano.c
// Runs on LM4F120 or TM4C123, 
// edX lab 13 
// There are four keys in the piano
// Daniel Valvano
// December 29, 2014

// Port E bits 3-0 have 4 piano keys

#include "Piano.h"
#include "..//tm4c123gh6pm.h"

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

// **************Piano_Init*********************
// Initialize piano key inputs
// Input: none
// Output: none
void Piano_Init(void){ 
  unsigned long delay;
	SYSCTL_RCGC2_R |= 0x10; // Activate clock for port E
	delay = SYSCTL_RCGC2_R;
	GPIO_PORTE_DATA_R = 0x400053FC;
	GPIO_PORTE_LOCK_R = 0x4C4F434B; // Unlock GPIO port E
	GPIO_PORTE_CR_R = 0x0F; // Allow changes to PE0 to PE3
	GPIO_PORTE_AMSEL_R = 0x00; // Disable analog for PE
	GPIO_PORTE_PCTL_R = 0x00; // Disable alternate functions
	GPIO_PORTE_DIR_R = 0x00; // PE0 to PE3 are inputs
	GPIO_PORTE_AFSEL_R = 0x00; // Disable alternate functions
	GPIO_PORTE_DEN_R = 0x3F; // Digital enable for PE0 to PE3
	
}
// **************Piano_In*********************
// Input from piano key inputs
// Input: none 
// Output: 0 to 15 depending on keys
// 0x01 is key 0 pressed, 0x02 is key 1 pressed,
// 0x04 is key 2 pressed, 0x08 is key 3 pressed
unsigned long Piano_In(void){
	
	if(GPIO_PORTE_DATA_R == 0x01) {
		return C;
	} else if(GPIO_PORTE_DATA_R == 0x02) {
		return D;
	} else if(GPIO_PORTE_DATA_R == 0x04) {
		return E;
	} else if(GPIO_PORTE_DATA_R == 0x08) {
		return G;
	} else {
		return NONE;
	}
	
}
