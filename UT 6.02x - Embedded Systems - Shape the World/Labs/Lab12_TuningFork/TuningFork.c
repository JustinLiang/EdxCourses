// TuningFork.c Lab 12
// Runs on LM4F120/TM4C123
// Use SysTick interrupts to create a squarewave at 440Hz.  
// There is a positive logic switch connected to PA3, PB3, or PE3.
// There is an output on PA2, PB2, or PE2. The output is 
//   connected to headphones through a 1k resistor.
// The volume-limiting resistor can be any value from 680 to 2000 ohms
// The tone is initially off, when the switch goes from
// not touched to touched, the tone toggles on/off.
//                   |---------|               |---------|     
// Switch   ---------|         |---------------|         |------
//
//                    |-| |-| |-| |-| |-| |-| |-|
// Tone     ----------| |-| |-| |-| |-| |-| |-| |---------------
//
// Daniel Valvano, Jonathan Valvano
// December 29, 2014

/* This example accompanies the book
   "Embedded Systems: Introduction to ARM Cortex M Microcontrollers",
   ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2014
   "Embedded Systems: Real Time Interfacing to ARM Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2014

 Copyright 2015 by Jonathan W. Valvano, valvano@mail.utexas.edu
    You may use, edit, run or distribute this file
    as long as the above copyright notice remains
 THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 VALVANO SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL,
 OR CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 For more information about my classes, my research, and my books, see
 http://users.ece.utexas.edu/~valvano/
 */


#include "TExaS.h"
#include "..//tm4c123gh6pm.h"


// basic functions defined at end of startup.s
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void WaitForInterrupt(void);  // low power mode

int toggleFlag = 0;
int edge = 0;
int debounce = 3;

// input from PA3, output from PA2, SysTick interrupts
void Sound_Init(void){ 
	
	unsigned long delay;
	SYSCTL_RCGC2_R |= 0x01; // Activate clock for port A
	delay = SYSCTL_RCGC2_R;
	GPIO_PORTA_DATA_R = 0x400053FC;
	GPIO_PORTA_LOCK_R = 0x4C4F434B; // Unlock GPIO port B
	GPIO_PORTA_CR_R = 0x0C; // Allow changes to PA2 and PA3
	GPIO_PORTA_AMSEL_R = 0x00; // Disable analog for PB
	GPIO_PORTA_PCTL_R = 0x00; // Disable alternate functions
	GPIO_PORTA_DIR_R = 0x04; // 2 is an output
	GPIO_PORTA_AFSEL_R = 0x00; // Disable alternate functions
	GPIO_PORTA_DEN_R = 0x0C; // Digital enable for 2 and 3
	
	// SysTick
	NVIC_ST_CTRL_R = 0x00; // Disable SysTick during startup
	NVIC_ST_RELOAD_R = 90908; // Setup reload to be max possible value
	NVIC_ST_CURRENT_R = 0x00; // Set any value to reset it;
	NVIC_SYS_PRI3_R = NVIC_SYS_PRI3_R&0X00FFFFFF; // Priority of 0 for bits 29-31 [29-31 is for systick]
	NVIC_ST_CTRL_R = 0x07; // Enable SysTick with core clock and interrupts
	
}

// called at 880 Hz
void SysTick_Handler(void){	
	
	if(toggleFlag) {
		
		GPIO_PORTA_DATA_R ^= 0x04; // toggle PA2
		
	}
	
}

void Delay1ms(unsigned long msec){

	unsigned long i;
  while(msec > 0){
    i = 13333;  // this number means 100ms
    while(i > 0){
      i = i - 1;
    }
    msec = msec - 1; // decrements every 100 ms
  }
	
}

int main(void){// activate grader and set system clock to 80 MHz
  TExaS_Init(SW_PIN_PA3, HEADPHONE_PIN_PA2,ScopeOn); 
  Sound_Init();         
  EnableInterrupts();   // enable after all initialization are done
  while(1){
		
		// main program is free to perform other tasks
    // do not use WaitForInterrupt() here, it may cause the TExaS to crash
		
		// If rising edge, edge is true
		if(edge == 0 && ((GPIO_PORTA_DATA_R & 0x08) == 0x08)) {
			
			Delay1ms(5);
			edge = 1;
			toggleFlag = !toggleFlag;
			
			
		}
		
		// If falling edge, toggle the flag and reset the edge
		if(edge == 1 && ((GPIO_PORTA_DATA_R & 0x08) == 0x00)) {
		
			Delay1ms(5);
			edge = 0;
			
		}
		
		if(toggleFlag == 0) {
			
			GPIO_PORTA_DATA_R &= ~0x04;
			
		}
	
  }
}
