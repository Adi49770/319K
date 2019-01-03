// EdgeInterrupt.c
// Runs on LM4F120 or TM4C123
// Request an interrupt on the falling edge of PF4 (when the user
// button is pressed) and increment a counter in the interrupt.  Note
// that button bouncing is not addressed.
// Daniel Valvano
// September 14, 2013

/* This example accompanies the book
   "Embedded Systems: Introduction to ARM Cortex M Microcontrollers"
   ISBN: 978-1469998749, Jonathan Valvano, copyright (c) 2014
   Volume 1, Program 9.4
   
   "Embedded Systems: Real Time Interfacing to ARM Cortex M Microcontrollers",
   ISBN: 978-1463590154, Jonathan Valvano, copyright (c) 2014
   Volume 2, Program 5.6, Section 5.5

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

// user button connected to PF4 (increment counter on falling edge)

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "EdgeInterrupt.h"
#include "ST7735.h"

void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
long StartCritical (void);    // previous I bit, disable interrupts
void EndCritical(long sr);    // restore I bit to previous value
void WaitForInterrupt(void);  // low power mode

// global variable visible in Watch window of debugger
// increments at least once per button press
int flag = 0;
int PD0 = 0;
int PD1 = 0;
int PD2 = 0;


void Buttons_Init(void){                          
  SYSCTL_RCGCGPIO_R |= 0x00000008; // (a) activate clock for port D
	while((SYSCTL_PRGPIO_R&0x8) == 0){}// (b) initialize counter
  GPIO_PORTD_DIR_R &= ~0x07;    // (c) make PF4 in (built-in button)
  GPIO_PORTD_AFSEL_R &= ~0x7;  //     disable alt funct on PF4 
	GPIO_PORTD_AMSEL_R = 0;       //     disable analog functionality on PF
	GPIO_PORTD_IM_R &= ~0x07;
  GPIO_PORTD_IS_R &= ~0x07;     // (d) PF4 is edge-sensitive
  GPIO_PORTD_IBE_R &= ~0x07;    //     PF4 is not both edges
  GPIO_PORTD_IEV_R |= 0x07;    //     PD1 rising edge event
  GPIO_PORTD_ICR_R = 0x07;      // (e) clear flag4
        // (f) arm interrupt on PF4 *** No IME bit as mentioned in Book ***
  NVIC_PRI0_R = (NVIC_PRI0_R&0x00FFFFFF)|0xA0000000; // (g) priority 5
  NVIC_EN0_R = 0x00000008;      // (h) enable interrupt 30 in NVIC
	GPIO_PORTD_IM_R |= 0x07;
	GPIO_PORTD_DEN_R |= 0x07;     //     enable digital I/O on PF4  
}

void GPIOPortD_Handler(void){
	if (flag==1) return; //check bounce flag
	int buttons = GPIO_PORTD_RIS_R&0x00000007; // get button data
	
	//Do something based on button press
	switch(buttons){
		case 1: PD0 = 1; break;
		case 2: PD1 = 1; break;
		case 4: PD2 = 1; break;
		
		case 3: PD0 = 1;PD1=1;break;
		case 5: PD0=1;PD2=1;break;
		case 6: PD1=1;PD2=1;break;
		case 7: PD0=1;PD2=1;PD1=1;break;
	}
	flag = 1;
	GPIO_PORTD_ICR_R |= 0x07;      // acknowledge flag4, RIS cleared by ICR
}

