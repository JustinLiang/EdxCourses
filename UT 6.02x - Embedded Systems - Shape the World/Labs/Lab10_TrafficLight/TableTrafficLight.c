// ***** 0. Documentation Section *****
// TableTrafficLight.c for Lab 10
// Runs on LM4F120/TM4C123
// Index implementation of a Moore finite state machine to operate a traffic light.  
// Daniel Valvano, Jonathan Valvano
// December 29, 2014

// east/west red light connected to PB5
// east/west yellow light connected to PB4
// east/west green light connected to PB3
// north/south facing red light connected to PB2
// north/south facing yellow light connected to PB1
// north/south facing green light connected to PB0
// pedestrian detector connected to PE2 (1=pedestrian present)
// north/south car detector connected to PE1 (1=car present)
// east/west car detector connected to PE0 (1=car present)
// "walk" light connected to PF3 (built-in green LED)
// "don't walk" light connected to PF1 (built-in red LED)

// ***** 1. Pre-processor Directives Section *****
#include "TExaS.h"
#include "tm4c123gh6pm.h"
#include "TExaSscope.h"

// ***** 2. Global Declarations Section *****

// Linked data structure
struct State {
	unsigned long outB;
	unsigned long outF;
	unsigned long delay;
	unsigned long next[9];
};

//typedef State *StatePtr;

#define NSGreen 0
#define NSYellow 1
#define EWGreen 2
#define EWYellow 3
#define WalkGo 4
#define WalkWarningOn1 5
#define WalkWarningOff1 6
#define WalkWarningOn2 7
#define WalkWarningOff2 8

struct State fsm[9] = {
	{0x21, 0x02, 300, {NSGreen, NSYellow, NSGreen, NSYellow, NSYellow,
		NSYellow, NSYellow, NSYellow}},
	{0x22, 0x02, 200, {NSYellow, EWGreen, NSGreen, EWGreen, WalkGo, 
		EWGreen, WalkGo, EWGreen}},
	{0x0C, 0x02, 300, {EWGreen, EWGreen, EWYellow, EWYellow, EWYellow,
		EWYellow, EWYellow, EWYellow}},
	{0x14, 0x02, 200, {EWYellow, EWGreen, NSGreen, NSGreen, WalkGo,
		WalkGo, NSGreen, WalkGo}},
	{0x24, 0x08, 300, {WalkGo, WalkWarningOn1, WalkWarningOn1, WalkWarningOn1,
		WalkGo, WalkWarningOn1, WalkWarningOn1, WalkWarningOn1}},
	{0x24, 0x02, 100, {WalkGo, WalkWarningOff1, WalkWarningOff1, WalkWarningOff1, 
		WalkGo, WalkWarningOff1, WalkWarningOff1, WalkWarningOff1}},
	{0x24, 0x00, 100, {WalkGo, WalkWarningOn2, WalkWarningOn2, WalkWarningOn2,
		WalkGo, WalkWarningOn2, WalkWarningOn2, WalkWarningOn2}},
	{0x24, 0x02, 100, {WalkGo, WalkWarningOff2, WalkWarningOff2, WalkWarningOff2, 
		WalkGo, WalkWarningOff2, WalkWarningOff2, WalkWarningOff2}},
	{0x24, 0x00, 100, {WalkGo, EWGreen, NSGreen, NSGreen, WalkGo,
		EWGreen, NSGreen, NSGreen}}
};

// FUNCTION PROTOTYPES: Each subroutine defined
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
unsigned long GetInput(void); // Get input
void SetOutputB(unsigned long outputB);
void SetOutputF(unsigned long outputF);

void PortB_Init(unsigned long delay) {
	
	SYSCTL_RCGC2_R |= 0x02; // Activate clock for port B
	delay = SYSCTL_RCGC2_R;
	GPIO_PORTB_DATA_R = 0x400053FC;
	GPIO_PORTB_LOCK_R = 0x4C4F434B; // Unlock GPIO port B
	GPIO_PORTB_CR_R = 0x3F; // Allow changes to PB0 - PB5
	GPIO_PORTB_AMSEL_R = 0x00; // Disable analog for PB
	GPIO_PORTB_PCTL_R = 0x00; // Disable alternate functions
	GPIO_PORTB_DIR_R = 0x3F; // 0 - 5 are outputs
	GPIO_PORTB_AFSEL_R = 0x00; // Disable alternate functions
	GPIO_PORTB_DEN_R = 0x3F; // Digital enable for 0 - 5
	
}

void PortE_Init(unsigned long delay) {
	
	SYSCTL_RCGC2_R |= 0x10; // Activate clock for port E
	delay = SYSCTL_RCGC2_R;
	GPIO_PORTE_DATA_R = 0X0400243FC;
	GPIO_PORTE_LOCK_R = 0x4C4F434B; // Unlock GPIO port E
	GPIO_PORTE_CR_R = 0x07; // Allow changes to PE0 - PE2
	GPIO_PORTE_AMSEL_R = 0x00; // Disable analog for PE
	GPIO_PORTE_PCTL_R = 0x00; // Disable alternate functions
	GPIO_PORTE_DIR_R = 0x00; // 0 - 2 are inputs
	GPIO_PORTE_AFSEL_R = 0x00; // Disable alternate functions
	GPIO_PORTE_DEN_R = 0x07; // Digital enable for 0 - 2
	
}

void PortF_Init(unsigned long delay) {
	
	SYSCTL_RCGC2_R |= 0x20; // Activate clock for port F
	delay = SYSCTL_RCGC2_R;
	GPIO_PORTF_DATA_R = 0X400253FC;
	GPIO_PORTF_LOCK_R = 0x4C4F434B; // Unlock GPIO port F
	GPIO_PORTF_CR_R = 0x0A; // Allow changes to PF1 and PF3
	GPIO_PORTF_AMSEL_R = 0x00; // Disable analog for PF
	GPIO_PORTF_PCTL_R = 0x00; // Disable alternate functions
	GPIO_PORTF_DIR_R = 0x0A; // 1 and 3 are outputs
	GPIO_PORTF_AFSEL_R = 0x00; // Disable alternate functions
	GPIO_PORTF_DEN_R = 0x0A; // Digital enable for 1 and 3
	
}

void SysTick_Init(void) {
	
		NVIC_ST_CTRL_R = 0x00; // Disable SysTick during startup
		NVIC_ST_RELOAD_R = 0x00FFFFFF; // Setup reload to be max possible value
		NVIC_ST_CURRENT_R = 0x00; // Set any value to reset it;
		NVIC_ST_CTRL_R = 0x05; // Enable SysTick with core clock
		
}

void SysTick_Wait(unsigned long delay) {
	
	NVIC_ST_RELOAD_R = delay - 1;
	NVIC_ST_CURRENT_R = 0;
	while((NVIC_ST_CURRENT_R&0x00010000)==0) {
	}

}

void SysTick_Wait_10ms(unsigned long delay) {
	
	unsigned long i;
	for(i = 0; i < delay; i++) {
		
		SysTick_Wait(800000);
		
	}
	
}

// ***** 3. Subroutines Section *****
unsigned long S; // index to the current state;
unsigned long input;
unsigned long outputB;
unsigned long outputF;
int main(void){ 
	unsigned long volatile delay;	
  TExaS_Init(SW_PIN_PE210, LED_PIN_PB543210); // activate grader and set system clock to 80 MHz
	//TExaS_Scope();
  PortB_Init(delay);
	PortE_Init(delay);
	PortF_Init(delay);
	SysTick_Init();
  
  EnableInterrupts();
  while(1){
   
		outputB = fsm[S].outB;
		outputF = fsm[S].outF;
		SetOutputB(outputB);
		SetOutputF(outputF);
		SysTick_Wait_10ms(fsm[S].delay*10);
	
		
		input = GetInput();
		S = fsm[S].next[input];

  }
}

unsigned long GetInput() {
	
	return GPIO_PORTE_DATA_R & 0x07;
	
}

void SetOutputB(unsigned long outputB) {
	
	GPIO_PORTB_DATA_R = outputB;
	
}

void SetOutputF(unsigned long outputF) {
	
	GPIO_PORTF_DATA_R = outputF;
	
}