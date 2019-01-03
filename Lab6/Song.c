#include "tm4c123gh6pm.h"
#include <stdint.h>
#include "Song.h"
#include "Sound.h"
#include "Piano.h"

#define quarter 1200000
#define triplet quarter*1/3
#define eighth quarter/1.5
#define half quarter*2
#define sixt eighth/16

long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value

//define notes here
Song amazingGrace[41]={
	{rest,sixt},{c,eighth}, {F,quarter},{A,triplet},{G,triplet},{F, triplet},{A,quarter}, {G,eighth}, {F, quarter}, {D,eighth}, {c, quarter}, {rest,sixt},//Phrase 1
	{c,eighth}, {F,quarter},{A,triplet},{G,triplet},{F, triplet},{A,quarter}, {G,eighth}, {A, eighth}, {C,half},{rest,sixt},//Phrase 2
	{A,eighth}, {C,quarter},{A,triplet},{G,triplet},{F, triplet},{A,quarter},{G,eighth}, {F, quarter}, {D,eighth}, {c, quarter},{rest,sixt},//Phrase 3
	{c,eighth}, {F,quarter},{A,triplet},{G,triplet},{F, triplet},{A,quarter}, {G,eighth}, {F, half}//Phrase 4	
};

void Song_Init(uint32_t period){ volatile unsigned int delay = 0;
	
	//Initialize Timer0A
	long sr;
  sr = StartCritical(); 
	SYSCTL_RCGCTIMER_R |= 0x01;   // 0) activate TIMER0
   for(int i=0;i<100;i++){delay++;}	// delay
  TIMER0_CTL_R = 0x00000000;    // 1) disable TIMER0A during setup
  TIMER0_CFG_R = 0x00000000;    // 2) configure for 32-bit mode
  TIMER0_TAMR_R = 0x00000002;   // 3) configure for periodic mode, default down-count settings
  TIMER0_TAILR_R = period-1;    // 4) reload value
  TIMER0_TAPR_R = 0;            // 5) bus clock resolution
  TIMER0_ICR_R = 0x00000001;    // 6) clear TIMER0A timeout flag
  TIMER0_IMR_R = 0x00000001;    // 7) arm timeout interrupt
  NVIC_PRI4_R = (NVIC_PRI4_R&0x00FFFFFF)|0x80000000; // 8) priority 4
// interrupts enabled in the main program after all devices initialized
// vector number 35, interrupt number 19
  NVIC_EN0_R = 1<<19;           // 9) enable IRQ 19 in NVIC	
	TIMER0_CTL_R = 0x00000001;    // 10) enable TIMER0A	
	EndCritical(sr);
}

int y=0;
int prevState = 0;
void Song_Play(uint8_t x){

	TIMER0_TAILR_R = amazingGrace[y].duration;
	
	if(prevState >= 50){
		TIMER0_CTL_R = 0x00000000;//disable timer
		Sound_Play(0);
	}

	prevState++;
}

void Song_Stop(void){
	TIMER0_TAILR_R = 0x00000000;//disable timer
  Sound_Play(1);//stop sound
}

void Timer0A_Handler(void){
  TIMER0_ICR_R = TIMER_ICR_TATOCINT;// acknowledge timer0A timeout
	for(int i=0;i<amazingGrace[y].duration;i++){
		Sound_Play(amazingGrace[y].note);
		
		if(y>=41||(y>1&&Piano_In()>=8)) {
		y=0;
		Song_Stop();
		for(int delay=0;delay<10000000;delay++){
			delay++;
		}
		return;
		}
	}
	y++;
}

