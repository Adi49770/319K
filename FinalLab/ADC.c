// ADC.c
// Runs on LM4F120/TM4C123
// Provide functions that initialize ADC0 on PE2
// Last Modified: 3/6/2015 
// Student names: Jerry Yang/Aditya Tyagi

#include <stdint.h>
#include "ADC.h"
#include "tm4c123gh6pm.h"
void PE_Init(void);

// ADC initialization function 
// Input: none
// Output: none
void ADC_Init(void){ volatile unsigned long delay = 0;
	//PE_Init();
  SYSCTL_RCGCADC_R |= 0x00000001; // 1) activate ADC0
  SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R4; // 1) activate clock for Port E
  delay = SYSCTL_RCGCGPIO_R;      // 2) allow time for clock to stabilize
  delay = SYSCTL_RCGCGPIO_R;
  GPIO_PORTE_DIR_R &= ~0x30;      // 3) make PE4 PE5 input
  GPIO_PORTE_AFSEL_R |= 0x30;     // 4) enable alternate function on PE4 PE5
  GPIO_PORTE_DEN_R &= ~0x30;      // 5) disable digital I/O on PE4 PE5
                                  // 5a) configure PE4 as ?? (skip this line because PCTL is for digital only)
  GPIO_PORTE_PCTL_R = GPIO_PORTE_PCTL_R&0xFF00FFFF;
  GPIO_PORTE_AMSEL_R |= 0x30;     // 6) enable analog functionality on PE4 PE5
  ADC1_PC_R &= ~0xF;              // 8) clear max sample rate field
  ADC1_PC_R |= 0x1;               //    configure for 125K samples/sec
  ADC1_SSPRI_R = 0x3210;          // 9) Sequencer 3 is lowest priority
  ADC1_ACTSS_R &= ~0x0004;        // 10) disable sample sequencer 2
  ADC1_EMUX_R &= ~0x0F00;         // 11) seq2 is software trigger
  ADC1_SSMUX2_R = 0x0089;         // 12) set channels for SS2
  ADC1_SSCTL2_R = 0x0060;         // 13) no TS0 D0 IE0 END0 TS1 D1, yes IE1 END1
  ADC1_IM_R &= ~0x0004;           // 14) disable SS2 interrupts
  ADC1_ACTSS_R |= 0x0004;         // 15) enable sample sequencer 2
	
}

//------------ADC_In------------
// Busy-wait Analog to digital conversion
// Input: none
// Output: 12-bit result of ADC conversion
int ADC_In(void){  uint32_t result;
  ADC1_PSSI_R = 0x04;           		// 1) initiate SS3
  while((ADC1_RIS_R&0x04)==0){};   // 2) wait for conversion done
  result = ADC1_SSFIFO2_R&0xFFF;   // 3) read result
	result = ADC1_SSFIFO2_R&0xFFF;
  ADC1_ISC_R = 0x004;             // 4) acknowledge completion
  return result;
}


// PE2 initialization function for ADC
// Input: none
// Output: none
void PE_Init(void){
		SYSCTL_RCGCGPIO_R |= 0x10;
	while( (SYSCTL_PRGPIO_R&0x10)==0){};
	GPIO_PORTE_DIR_R &= ~0x4;
	GPIO_PORTE_AFSEL_R |= 0x04;
	GPIO_PORTE_DEN_R &= ~0x04;
	GPIO_PORTE_AMSEL_R |= 0x04;
}
