// Sound.h
// Runs on TM4C123 or LM4F120
// Prototypes for basic functions to play sounds from the
// original Space Invaders.
// Jonathan Valvano
// November 17, 2014
#include <stdint.h>

void Sound_Init(void);
void Sound_Play(const uint8_t *pt, uint32_t count);
void Sound_In(void);
void Sound_Hit(void);
void Sound_HoleEnd(void);
void Sound_Water(void);
void Sound_Sand(void);

