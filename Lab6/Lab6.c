// Lab6.c
// Runs on LM4F120 or TM4C123
// Use SysTick interrupts to implement a 4-key digital piano
// MOOC lab 13 or EE319K lab6 starter
// Program written by: put your names here
// Date Created: 3/6/17 
// Last Modified: 3/6/17 
// Lab number: 6
// Hardware connections
// TO STUDENTS "REMOVE THIS LINE AND SPECIFY YOUR HARDWARE********


#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "Sound.h"
#include "Piano.h"
#include "TExaS.h"
#include "Song.h"
#include "dac.h"

// basic functions defined at end of startup.s
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void Heartbeat_Init(void);

int main(void){ 
  TExaS_Init(SW_PIN_PE3210,DAC_PIN_PB3210,ScopeOn);    // bus clock at 80 MHz
  Piano_Init();
  Sound_Init(0);
	Song_Init(1);
  Heartbeat_Init();
  EnableInterrupts();
  while(1){
		GPIO_PORTF_DATA_R ^= 0x8; //heartbeat
		for(int delay=0;delay<1000000;delay++){
			delay++;
		}
	
		switch(Piano_In()){
			case 0: Sound_Play(1); break;//CHANGE PERIODS HERE
			case 1: Sound_Play(c); break;
			case 2: Sound_Play(D); break;
			case 3: Sound_Play(E); break;
			case 4: Sound_Play(F); break;
			case 5: Sound_Play(G); break;
			case 6: Sound_Play(A); break;
			case 7: Sound_Play(B); break;
			case 8: GPIO_PORTF_DATA_R = 0x8; //heartbeat
				Song_Play(1);	break;
			case 15: Sound_Play(C); break;
			default: Sound_Play(1);
		}
  }    
}

void Heartbeat_Init(){ volatile unsigned int delay =0;
	SYSCTL_RCGC2_R |= 0x20;
	delay++; delay++;
	GPIO_PORTF_DIR_R = 0x8;
	GPIO_PORTF_AFSEL_R = 0x0;
	GPIO_PORTF_AMSEL_R = 0x0;
	GPIO_PORTF_DEN_R = 0x8;
}
	
