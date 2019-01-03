#include <stdint.h>
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

#define rest 0

 struct notes {
	uint32_t note;
	uint32_t duration;
};

typedef const struct notes Song;

void Song_Init(uint32_t x);

void Song_Play(uint8_t x);

void Song_Stop(void);
