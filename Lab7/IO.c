// IO.c
// This software configures the switch and LED
// You are allowed to use any switch and any LED, 
// although the Lab suggests the SW1 switch PF4 and Red LED PF1
// Runs on LM4F120 or TM4C123
// Program written by: put your names here
// Date Created: 
// Last Modified:  
// Lab number: 6


#include "tm4c123gh6pm.h"
#include <stdint.h>

//------------IO_Init------------
// Initialize GPIO Port for a switch and an LED
// Input: none
// Output: none
void IO_Init(void) {
 // --UUU-- Code to initialize PF4 and PF2
	SYSCTL_RCGC2_R |= 0x20; // enable clock for PF
	volatile int delay;
	delay++; delay++;
	GPIO_PORTF_AFSEL_R = 0x0;
	GPIO_PORTF_AMSEL_R = 0x0;
	GPIO_PORTF_DIR_R &=0xEF; // set PF4 to input (positive logic)
	GPIO_PORTF_DIR_R |= 0x4; // set PF2 to output
	GPIO_PORTF_PUR_R |= 0x10;
	GPIO_PORTF_DEN_R = 0x14; // enable PF2
}

//------------IO_HeartBeat------------
// Toggle the output state of the  LED.
// Input: none
// Output: none
void IO_HeartBeat(void) {
 // --UUU-- PF2 is heartbeat
	GPIO_PORTF_DATA_R ^= 0x04; //heartbeat at PF2
		
	
}


//------------IO_Touch------------
// wait for release and press of the switch
// Delay to debounce the switch
// Input: none
// Output: none
void IO_Touch(void) {
	
	while( (GPIO_PORTF_DATA_R & 0x00000010) ){	} //wait for release
	
	//delay for 20 ms
	for(int y = 0; y<2000; y++){ // this goes 2000 times. 2000 * 0.01ms = 20ms
		for(int x = 0 ; x<1600; x++){ //ball parked the number for 0.01ms
		}
	}

	while( (1) ){	
		volatile int data = GPIO_PORTF_DATA_R;
		data&=0x10;
		data = data>>4;
		if(data==0)
			break;
	} //wait for press
}
 // --UUU-- wait for release; delay for 20ms; and then wait for press
 

