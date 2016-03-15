// ***** 0. Documentation Section *****
// SwitchLEDInterface.c for Lab 8
// Runs on LM4F120/TM4C123
// Use simple programming structures in C to toggle an LED
// while a button is pressed and turn the LED on when the
// button is released.  This lab requires external hardware
// to be wired to the LaunchPad using the prototyping board.
// December 28, 2014
//      Jon Valvano and Ramesh Yerraballi

// ***** 1. Pre-processor Directives Section *****
#include "TExaS.h"
#include "tm4c123gh6pm.h"

// ***** 2. Global Declarations Section *****

#define GPIO_PORTE_CR_R (*((volatile unsigned long *)0x40024524))
#define GPIO_PORTE_DATA_R (*((volatile unsigned long *)0x400243FC))
#define GPIO_PORTE_DIR_R (*((volatile unsigned long *)0x40024400))
#define GPIO_PORTE_AFSEL_R (*((volatile unsigned long *)0x40024420))
#define GPIO_PORTE_DEN_R (*((volatile unsigned long *)0x4002451C))
#define GPIO_PORTE_LOCK_R (*((volatile unsigned long *)0x40024520))
#define GPIO_PORTE_AMSEL_R (*((volatile unsigned long *)0x40024528))
#define GPIO_PORTE_PCTL_R (*((volatile unsigned long *)0x4002452C))
#define SYSCTL_RCGC2_R (*((volatile unsigned long *)0x400FE108))

// FUNCTION PROTOTYPES: Each subroutine defined
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void PortE_init(unsigned long delay);
void Delay100ms(unsigned long time);

// ***** 3. Subroutines Section *****

// PE0, PB0, or PA2 connected to positive logic momentary switch using 10k ohm pull down resistor
// PE1, PB1, or PA3 connected to positive logic LED through 470 ohm current limiting resistor
// To avoid damaging your hardware, ensure that your circuits match the schematic
// shown in Lab8_artist.sch (PCB Artist schematic file) or 
// Lab8_artist.pdf (compatible with many various readers like Adobe Acrobat).

int main(void){ unsigned long volatile delay;	
//**********************************************************************
// The following version tests input on PE0 and output on PE1
//**********************************************************************
  TExaS_Init(SW_PIN_PE0, LED_PIN_PE1);  // activate grader and set system clock to 80 MHz
  PortE_init(delay);
	
	GPIO_PORTE_DATA_R |= 0x02;
	
	EnableInterrupts();           // enable interrupts for the grader
	
	while(1){
		Delay100ms(1);	
		
		if((GPIO_PORTE_DATA_R & 0x01) == 0x01) { // if the switch is on
			
			if((GPIO_PORTE_DATA_R & 0x02) == 0x02) { // if the LED is on
				
				GPIO_PORTE_DATA_R &=~ 0x02; // turn the LED off
				
			} else {
				
				GPIO_PORTE_DATA_R |= 0x02; // turn it on
				
			}
			
		} else {
			
				GPIO_PORTE_DATA_R |= 0x02; // turn it on

		}
		
	}
		  
}

void PortE_init(unsigned long delay) {

	SYSCTL_RCGC2_R = 0x10; // Turn clock on for port E
	delay = SYSCTL_RCGC2_R; // Create delay to allow for clock to turn on
	GPIO_PORTE_LOCK_R = 0x4C4F434B; // Unlock port E
	GPIO_PORTE_CR_R = 0x03; // Allow changes for PE0 and PE1
	GPIO_PORTE_AMSEL_R = 0x00; // Disable analog
	GPIO_PORTE_PCTL_R = 0x00000000; // Reset PCTL
	GPIO_PORTE_DIR_R = 0x02; // PE1 is an output and PE1 is an input
	GPIO_PORTE_AFSEL_R = 0x00000000; // Disable alternative functions
	GPIO_PORTE_DEN_R = 0x03; // Enable digital for PE0 and PE1
	
}

void Delay100ms(unsigned long time){
  unsigned long i;
  while(time > 0){
    i = 1333333;  // this number means 100ms
    while(i > 0){
      i = i - 1;
    }
    time = time - 1; // decrements every 100 ms
  }
}