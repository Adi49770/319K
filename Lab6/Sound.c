// Sound.c
// This module contains the SysTick ISR that plays sound
// Runs on LM4F120 or TM4C123
// Program written by: put your names here
// Date Created: 3/6/17 
// Last Modified: 3/6/17 
// Lab number: 6
// Hardware connections
// TO STUDENTS "REMOVE THIS LINE AND SPECIFY YOUR HARDWARE********

// Code files contain the actual implemenation for public functions
// this file also contains an private functions and private data
#include <stdint.h>
#include "dac.h"
#include "tm4c123gh6pm.h"

// **************Sound_Init*********************
// Initialize Systick periodic interrupts
// Called once, with sound initially off
// Input: interrupt period
//           Units to be determined by YOU
//           Maximum to be determined by YOU
//           Minimum to be determined by YOU
// Output: none
void Sound_Init(uint32_t period){
  NVIC_ST_CTRL_R &=0x0;
	NVIC_ST_RELOAD_R = period-1;
	NVIC_ST_CURRENT_R = 0;
	NVIC_SYS_PRI3_R = (NVIC_SYS_PRI3_R&0x00FFFFFF)|0x40000000; //Set priority 2
	NVIC_ST_CTRL_R |= 0x7;//Start and arm
	DAC_Init();
}


// **************Sound_Play*********************
// Start sound output, and set Systick interrupt period 
// Input: interrupt period
//           Units to be determined by YOU
//           Maximum to be determined by YOU
//           Minimum to be determined by YOU
//         input of zero disable sound output
// Output: none

const unsigned char Flute[64] = {1,2,3,4,5,6,8,10,12,16,20,22,24,26,28,30,35,40,45,50,53,56,60,
62,63,62,60,56,52,50,48,44,40,36,32,28,26,25,25,25,25,24,23,22,21,
19,17,16,15,12,11,10,9,8,7,6,5,4,2,1,1,0,0,0};

//double amplitude = 0;
int changeAmp = 0;
int prevPeriod = 1;
void Sound_Play(uint32_t period){
	NVIC_ST_RELOAD_R = period-1;
	/*if(prevPeriod == period) {
		for(int i=0; i<100;i++){}
		amplitude+=1;}*/
  if(prevPeriod!=period) {//amplitude = 1;}
	prevPeriod = period;
	
		//delay
	//for(int i=0;i<10000;i++){}
}


int val=64;
void SysTick_Handler(void);{//Interrupt service routine
	if( val < 64){
		DAC_Out(Flute[val]);
		//DAC_Out(Flute[val]/amplitude);
		val++;
	}else{
		val = 0;
		if(changeAmp >= 100){
		//	amplitude++;
			changeAmp=0;
		}else{
			changeAmp++;
		}
		DAC_Out(val);
	}
}
}

