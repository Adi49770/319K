// TableTrafficLight.c solution to edX lab 10, EE319KLab 5
// Runs on LM4F120 or TM4C123
// Index implementation of a Moore finite state machine to operate a traffic light.  
// Daniel Valvano, Jonathan Valvano
// November 7, 2013

/* solution, do not post

 Copyright 2014 by Jonathan W. Valvano, valvano@mail.utexas.edu
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

// east/west red light connected to PA7
// east/west yellow light connected to PA6
// east/west green light connected to PA5
// north/south facing red light connected to PA4
// north/south facing yellow light connected to PA3
// north/south facing green light connected to PA2
// pedestrian detector connected to PE2 (1=pedestrian present)
// north/south car detector connected to PE1 (1=car present)
// east/west car detector connected to PE0 (1=car present)
// "walk" light connected to PF3 (built-in green LED)
// "don't walk" light connected to PF1 (built-in red LED)

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "SysTick.h"
#include "TExaS.h"

// Declare your FSM linked structure here
#define goS 0
#define waitS 1
#define stopS 2
#define goW 3
#define waitW 4
#define stopW 5
#define waitWalkS 6
#define waitWalkW 7
#define walkStop 8
#define walkOn 9
#define walkWaitOn1 10
#define walkWaitOff1 11
#define walkWaitOn2 12
#define walkWaitOff2 13
#define walkWaitOn3 14
#define walkWaitOff3 15
#define walkWaitOn4 16
#define walkWaitOff4 17
#define afterWalk 18
#define numOfStates 19
//19 states
typedef const struct state{
	uint8_t output;
	//uint8_t PedOut;
	uint16_t delay;
	uint8_t next[8];
}STyp;

STyp FSM[19]={
{0x4C, 200, {waitS,	waitS	,waitS,	waitS,	waitWalkS,	waitWalkS,	waitWalkS,	waitS}}, //goS
{0x54, 100, {stopS,	stopS	,stopS	,stopS,	waitWalkS	,waitWalkS,	waitWalkS,	stopS}}, //waitS
{0x64, 200, {stopS,	goW	,goS,	goW,	walkOn,	walkOn,	walkOn,	goW}},//stopS
{0x61, 200, {waitW,	waitW,	waitW,	waitW,	waitWalkW,	waitWalkW,	waitWalkW,	waitW}},//goW
{0x62, 100, {stopW,	stopW	,stopW,	stopW	,waitWalkW	,waitWalkW	,waitWalkW	,stopW}},//waitW
{0x64, 200, {stopW,	goW,	goS,	goS,	walkOn,	walkOn,	walkOn,	walkOn}},//stopW
{0x54, 100, {walkStop, walkStop, walkStop,walkStop, walkStop, walkStop, walkStop, walkStop}},//waitWalkS
{0x62, 200, {walkStop, walkStop, walkStop,walkStop, walkStop, walkStop, walkStop, walkStop}},//waitWalkW
{0x64, 200, {walkOn, walkOn, walkOn, walkOn, walkOn, walkOn, walkOn, walkOn}},//walkStop
{0xA4, 200, {walkWaitOn1,walkWaitOn1,walkWaitOn1,walkWaitOn1,walkWaitOn1,walkWaitOn1,walkWaitOn1,walkWaitOn1}},//walkOn
{0x64, 50, {walkWaitOff1, walkWaitOff1, walkWaitOff1, walkWaitOff1, walkWaitOff1, walkWaitOff1,walkWaitOff1 ,walkWaitOff1}},//walkWaitOn1
{0x24, 50, {walkWaitOn2, walkWaitOn2, walkWaitOn2, walkWaitOn2, walkWaitOn2, walkWaitOn2, walkWaitOn2, walkWaitOn2}},//walkWaitOff1
{0x64, 50, {walkWaitOff2, walkWaitOff2, walkWaitOff2, walkWaitOff2,walkWaitOff2, walkWaitOff2, walkWaitOff2, walkWaitOff2}},//walkWaitOn2
{0x24, 50, {walkWaitOn3,walkWaitOn3,walkWaitOn3,walkWaitOn3,walkWaitOn3,walkWaitOn3,walkWaitOn3,walkWaitOn3}},//walkWaitOff2
{0x64, 50, {walkWaitOff3,walkWaitOff3,walkWaitOff3,walkWaitOff3,walkWaitOff3,walkWaitOff3,walkWaitOff3,walkWaitOff3}},//walkWaitOn3
{0x24, 50, {walkWaitOn4,walkWaitOn4,walkWaitOn4,walkWaitOn4,walkWaitOn4,walkWaitOn4,walkWaitOn4,walkWaitOn4 }},//walkWaitOff3
{0x64, 50, {walkWaitOff4,walkWaitOff4,walkWaitOff4,walkWaitOff4,walkWaitOff4,walkWaitOff4,walkWaitOff4,walkWaitOff4}},//walkWaitOn4
{0x24, 50, {afterWalk	,afterWalk,	afterWalk	,afterWalk,afterWalk,afterWalk,afterWalk,afterWalk}},//walkWaitOff4
{0x64, 200, {afterWalk,	goW,	goS,	goS,	goW,	goW,	goS,	goS}},//afterWalk
};

void EnableInterrupts(void);
void PortA_Init(void);
void PortE_Init(void);
void PortF_Init(void);
void blinkWalkLight(void);



int main(void){ volatile unsigned long delay;
	uint32_t currentState = goS; //Must put before SysTick_Init(), since busywait loop uses it as parameter
	uint32_t input = 0;
	
  TExaS_Init(SW_PIN_PE210, LED_PIN_PA765432); // activate traffic simulation and set system clock to 80 MHz
  SysTick_Init();     
  EnableInterrupts();
	
	PortA_Init();
	PortE_Init();
	PortF_Init();
	
  //FSM Engine - this is done; rather, it works based on my simulator tests.
  while(1){
		GPIO_PORTA_DATA_R = FSM[currentState].output <<2;
		GPIO_PORTF_DATA_R = ((FSM[currentState].output >>5)& 0x2) | ((FSM[currentState].output>>4)& 0x8);
		SysTick_Wait10ms(FSM[currentState].delay);
		input = (GPIO_PORTE_DATA_R & 0x00000007);
		currentState = FSM[currentState].next[input];
		
  }
}

//Initialization functions
void PortA_Init(void){
	volatile unsigned int delay=0;
	SYSCTL_RCGC2_R|=0x1;
	delay++; delay++;
	GPIO_PORTA_DIR_R  |=0xFC;
	GPIO_PORTA_AFSEL_R &=0x3;
	GPIO_PORTA_DEN_R |=0xFC;
}
void PortE_Init(void){
	volatile unsigned int delay=0;
	SYSCTL_RCGC2_R|=0x10;
	delay++;
	delay++;
	GPIO_PORTE_DIR_R &= 0x0;
	GPIO_PORTE_AFSEL_R &=0x8;
	GPIO_PORTE_AMSEL_R &= 0x8;
	GPIO_PORTE_DEN_R |=0x7;
}
void PortF_Init(void){
	volatile unsigned int delay=0;
	SYSCTL_RCGC2_R|=0x20;
	delay++;
	delay++;
	GPIO_PORTF_DIR_R |=0xA;
	GPIO_PORTF_AFSEL_R &=0x0;
	GPIO_PORTF_DEN_R |=0xA;
}
