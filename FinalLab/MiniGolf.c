#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "ST7735.h"
#include "Random.h"
#include "TExaS.h"
#include "Sound.h"
#include "ADC.h"
#include "Sensor.h"
#include "EdgeInterrupt.h"
#include "Physics.h"
//#include "MiniGolf.h"
#define Water 1
#define Sand 2
#define TreeA 3
#define TreeB 4
void PortFInit(void);
void ClearButtons(void);
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts
void Delay100ms(uint32_t count); // time delay in 0.1 seconds
int getStrokes(void);
void makeBallX(int x);
void makeBallY(int y);

// *************************** Images ***************************
// enemy ship that starts at the top of the screen (arms/mouth closed)
// width=16 x height=10

// *************************** Capture image dimensions out of BMP**********
int displayStart(void);
void displayHelp(void);
int getBallWidth(void); int getBallHeight(void); int getBallX(void); int getBallY(void);
void setBallWidth(int set); void setBallHeight(int set); void setBallX(int set); void setBallY(int set);
int state=0;
struct Object {int xPos; int yPos; uint16_t width; uint16_t height;};
void startGame(void);
typedef struct Object Ball;
typedef struct Object Tree;
typedef struct Object Player;
typedef struct Object Hole;
typedef struct Object sand;
typedef struct Object water;
int Strokes; 
Ball ball1; 
Tree treeA;
Tree treeB;
sand sand1;
water water1;
void FinalScreen(void);


int main(void){
	// setting the original location of the ball should be done with Physics.c file to avoid redudancy (calibration)
	DisableInterrupts();
  TExaS_Init();  // set system clock to 80 MHz
		ADC_Init();
  Random_Init(1);
  Output_Init();

	Sound_Init();
	Buttons_Init();
	Sensor_Init();
	PortFInit();
	EnableInterrupts();
	GPIO_PORTF_DATA_R ^=0x02; //toggle heartbeat
	
//	void Sound_Play(Hit, );
//	Sound_Water(); 
	//while(1){}
	
	//startGame();
	//displayHelp();
	//ST7735_FillScreen(0x00000);
//	displayLevel(1);
	//ST7735_FillRect(90, 50, 4, 4, 0x00);
	//displayLevel(1);
	//displayLevel(2);
	//displayStart();
	//IF YOU USE A BUTTON PRESS, CALL ClearButtons() TO CLEAR THEM!!!
	
	switch(displayStart()){
		case 0: startGame();break;
	//	case 1: selectLevel();break;
		case 1: displayHelp(); break;//Start Screen
	}
	//if(help ==1) displayStart();
	
	
	//ST7735_DrawBitmap(ball1.xPos, ball1.yPos, ball1.image, ball1.width, ball1.height);
	//ST7735_DrawBitmap(64, 149, ball[], Ball.width,Ball.height);
	GPIO_PORTF_DATA_R ^=0x02; // toggle heartbeat
  
}

int startArray[3] = {8,10,12};
int displayStart(){
	ST7735_FillScreen(0x0000);
	ST7735_SetCursor(4,2);
	ST7735_OutString(" HOLE IN FUN ");
	ST7735_SetCursor(4,3);
	ST7735_OutString(" MINIGOLF!!! ");
	ST7735_SetCursor(4,8);
	ST7735_OutString(" START GAME ");
	ST7735_SetCursor(4,10);
	ST7735_OutString(" HELP ");

	int index = 0;
	while(1){
		ST7735_SetCursor(3,startArray[index]);
		ST7735_OutChar(0x3E);
		Delay100ms(50);
		ST7735_SetCursor(3,startArray[index]);
		ST7735_OutChar(0x20);
		Delay100ms(50);
		if(PD0) {ClearButtons(); return index;}
		if(PD1) {ClearButtons(); index--;} // change index based on button press
		if(PD2) {ClearButtons(); index++;}
		if(index==-1){index=1;} // circular array
		if(index==2){index=0;}
	}
}

void displayHelp(){
	ST7735_SetCursor(0,0);
	ST7735_OutString("HELP");
	ST7735_SetCursor(0,2);
	ST7735_OutString(" Left button- Swing");
	ST7735_SetCursor(0,4);
	ST7735_OutString(" Slide pot- Power");
	ST7735_SetCursor(0,6);
	ST7735_OutString(" Knob pot- Direction");
	ST7735_SetCursor(0,8);
	ST7735_OutString(" Top button- Options");
	ST7735_SetCursor(0,10);
	ST7735_OutString(" Bottom button- Exit");
	
	ST7735_SetCursor(0,13);
	ST7735_OutString(" Press any button to\n go back.");
	
	while(!PD0 && !PD1 && !PD2){}
	ClearButtons();
		return;
}


void startGame(){
	int LevelNumber =1;
	displayLevel(LevelNumber);
	setBall(); // reset the ball to the original position
	while(!(PD2)){// as long as PD1 or PD2 is not pressed (going back to the main menu)
		//display level
		
		setBounds(LevelNumber); // setboundaries for the level map
			while(!PD0){ // while the "swing button is not pressed
		showDir(LevelNumber); // show the magnitude and direction of the player's ball
		}
			ClearButtons();
		Strokes++;
		displayData(LevelNumber);
		// move ball in that certain position and check for all sorts of collisions
		volatile int check = MoveBall(--LevelNumber);
		LevelNumber++;
		if(check ==7){ // check if hole touched
			LevelNumber++;
			if(LevelNumber==3){
			FinalScreen();
			break;}
			displayLevel(LevelNumber);
			setBall(); 
		}
		
	}
	ClearButtons();
	return;
}
// You can use this timer only if you learn how it works

void Delay100ms(uint32_t count){uint32_t volatile time;
  while(count>0){
    time = 70000;  // 0.1sec at 80 MHz
    while(time){
	  	time--;
    }
    count--;
  }
}
int getBallWidth(void)
{
return ball1.width; 
}
int getBallHeight(void){return ball1.height;}
int getBallX(void){return ball1.xPos;}
int getBallY(void){return ball1.yPos;}
void setBallWidth(int set){ball1.width = set;}
void setBallHeight(int set){ball1.height=set;}
void setBallX(int set){ball1.xPos+=set;}
void setBallY(int set){ball1.yPos+=set;}
int getXPos(int item) {
	switch (item)
	{
		case Water: return water1.xPos;
		case Sand: return sand1.xPos;
		case TreeA: return treeA.xPos;
		case TreeB: return treeB.xPos;
	}
	return 0;
}
int getYPos(int item)
{
	switch (item)
	{
		case Water: return water1.yPos;
		case Sand: return sand1.yPos;
		case TreeA: return treeA.yPos;
		case TreeB: return treeB.yPos;
	}
	return 0;
}
void setXPos(int item, int x)
{
	switch (item)
	{
		case Water: water1.xPos = x; break;
		case Sand: water1.xPos = x; break;
		case TreeA: water1.xPos = x; break;
		case TreeB: water1.xPos = x; break;
	}
}
void setYPos(int item, int y)
{
	switch (item)
	{
		case Water: water1.xPos = y; break;
		case Sand: water1.xPos = y; break;
		case TreeA: water1.xPos = y; break;
		case TreeB: water1.xPos = y; break;
	}
}
void PortFInit(void)
{
	volatile unsigned int delay=0;
	SYSCTL_RCGCGPIO_R|=0x21;
	delay++;
	delay++;
	delay++;
	delay++;
	GPIO_PORTF_DIR_R |=0x02; //PF2
	GPIO_PORTF_AFSEL_R &=0x0;
	GPIO_PORTF_DEN_R |=0x02;
}
void ClearButtons(){
	flag =0;
	PD0=0;
	PD1=0;
	PD2=0;
}
int getStrokes(void){return Strokes;}
void makeBallX(int x){ball1.xPos=x;}
void makeBallY(int y){ball1.yPos=y;}
void FinalScreen(void)
{
	ST7735_FillScreen(0x0000);
	ST7735_SetCursor(4,2);
	ST7735_OutString(" YOU WIN! ");
	while(!PD0)
	{
			ST7735_SetCursor(8,2);
	ST7735_OutString(" PRESS SWING FOR MAIN MENU ");
	}
	main();
}