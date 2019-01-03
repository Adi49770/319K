// Sound.h
// This module contains the SysTick ISR that plays sound
// Runs on LM4F120 or TM4C123
// Program written by: put your names here
// Date Created: 3/6/17 
// Last Modified: 3/6/17 
// Lab number: 6
// Hardware connections
#include <stdint.h>
// Header files contain the prototypes for public functions
// this file explains what the module does

// **************Sound_Init*********************
// Initialize Systick periodic interrupts
// Called once, with sound initially off
// Input: interrupt period
//           Units to be determined by YOU
//           Maximum to be determined by YOU
//           Minimum to be determined by YOU
// Output: none
void Sound_Init(uint32_t period);


// **************Sound_Play*********************
// Start sound output, and set Systick interrupt period 
// Input: interrupt period
//           Units to be determined by YOU
//           Maximum to be determined by YOU
//           Minimum to be determined by YOU
//         input of zero disable sound output
// Output: none
void Sound_Play(uint32_t period);

//Lower octave
#define e 3792
#define g 3189
#define a 2841
#define bb 2681
#define b 2531
#define c 2389

//Upper octave
#define D 2128
#define Dsharp 2009
#define E 1896
#define F 1790
#define Fsharp 1689
#define G 1594
#define A 1420
#define B 1265
#define C 1194


// **************SysTick_Handler*********************
// Start sound output, and set Systick interrupt period 
void SysTick_Handler(void);
