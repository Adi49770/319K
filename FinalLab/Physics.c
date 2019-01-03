#include <stdint.h>
#include "MiniGolf.h"
#include "ST7735.h"
#include "ADC.h"
#include "Sound.h"
#define Water 1
#define Sand 2
#define TreeA 3
#define TreeB 4
//int a = -10;
//void Collide(struct Object obj1, struct Object obj2);
int score = 0;
int level = 1;
int getSpeed(void);
int getDir(void); 
int Convert(uint32_t input);
int ConvertDir(uint32_t input); 
void setBall(void);
void showDir(int level);
void showSpeed(void);
int BoundX; int BoundY; int BoundWidth; int BoundHeight;
void displayLevel(int level);
const short run[360] = {5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,1, 1, 1, 1, 1, 1, 1, 1, 1,0, 0, 0, 0, 0, 0,-1, -1, -1, -1, -1, -1, -1, -1, -1,-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,-3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3,-4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4,-5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5,-4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4,-3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3,-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,-1, -1, -1, -1, -1, -1, -1, -1, -1,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,1, 1, 1, 1, 1, 1, 1, 1, 1,2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5}; 
const short rise[360] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5, 5,4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 3,2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2,1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,-3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3,-4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4,-5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5, -5,-4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4, -4,-3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3, -3,-2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2, -2,-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,0, 0, 0, 0, 0};
//dir is value from 0 to 180
//speed is value from 0-10
int Collide(int level);

void setBall(void)
{
	makeBallX(30);
	makeBallY(100);
	setBallWidth(7);
	setBallHeight(7);
	ST7735_DrawBitmap(getBallX(),getBallY(),ball,7,7);
	//ST7735_DrawBitmap(getBallX(), getBallY(), ball,getBallWidth(),getBallHeight()); // set the ball to  the starting position. 
}
int MoveBall(int level){
	// make sure to return 1 if the collide results with hole to increase the levelnumber!
	Sound_Hit(); // make the hit sound
	int speed = getSpeed();
	int index = getDir();
	//int index = 19;// tester
	//int speed = 4; // tester
	int Rise = rise[index]; 
	int Run = run[index]; 
	//speed *= 1; //that is the factor t for now
	//volatile int x = 0; 
	int ballx =0; 
	int bally = 0; 
	while(speed>0)
	{
	//	ST7735_FillRect(getBallX()+ballx, getBallY()-bally, getBallWidth(), getBallHeight(), 0x07E0);
		ST7735_FillRect(getBallX(), getBallY() - 6, getBallWidth(), getBallHeight(), 0x07E0);
		ballx += Run;
		bally += Rise;
		volatile int checkery = getBallY();
		volatile int checkerx = getBallX();
		ST7735_DrawBitmap(getBallX()+ballx, getBallY()-bally, ball,getBallWidth(),getBallHeight());
		Delay100ms(12);
		volatile int check = Collide(level);
		switch(check)
		{
			case 0: setBallX(ballx); 
			if(index>0&& index<180) bally*=-1;
				setBallY(bally); ballx= 0; bally=0;speed--;continue;
			case Water: ST7735_FillRect(getBallX()+ballx, getBallY()-bally-7, getBallWidth(), getBallHeight(), 0x07E0);;
					ST7735_DrawBitmap(getBallX()-ballx, getBallY()+bally, ball,getBallWidth(),getBallHeight());
			speed =0;
					return Water; 
			case Sand: speed-=2;; return Sand; 
			case 7: return 7; 
			case TreeA: index+=180; index%=360;  
			ST7735_FillRect(getBallX()+ballx, getBallY()-bally-6, getBallWidth(), getBallHeight(), 0x07E0);
			Run = run[index]; Rise = rise[index]; speed--;
				continue;
			case TreeB: index+=180; index%=360;  
			ST7735_FillRect(getBallX()+ballx, getBallY()-bally-6, getBallWidth(), getBallHeight(), 0x07E0);
			Run = run[index]; Rise = rise[index];
			speed--;
			continue;
		}
		speed--;
	}
	return 0;
}
int Collide(int level)
{
	if((getBallX() >= 70 && getBallX() <=100)&& (getBallY()>=10 && getBallY()<= 50))
	{Sound_In();return 7;}
	switch (level)
	{
		case 1:  if (((getBallX() >= 50 && getBallX() <=60 + 15) && (getBallY()>=65 && getBallY()<= 85 + 15)) ||((getBallX() >= 5 && getBallX() <=5 + 15) && (getBallY()>=50 && getBallY()<= 75 + 15)) ) // water
								{Sound_Water(); return Water;}
						else if ((getBallX() >= 50 && getBallX() <=50 + 15) && (getBallY()>=60 && getBallY()<= 60 + 15)) //sand
								{Sound_Sand(); return Sand;}
								break;
		case 0: if ((getBallX() >= 5 && getBallX() <=5 + 15) && (getBallY()>=50 && getBallY()<= 75 + 15)) // water
							{Sound_Water();return Water;}
						else if ((getBallX() >= 50 && getBallX() <=60 + 15) && (getBallY()>=100 && getBallY()<= 100 + 15)) //sand
							{Sound_Sand();return Sand;}
							break;
	}
/*	if ((getBallX() >= (game[level].xTree1) && getBallX() <=(game[level].xTree1) + 2) && (getBallY()>=(game[level].yTree1) && getBallY()<= (game[level].yTree1) + 2)) // treeA
	{Sound_Hit();return TreeA;}	
		else if ((getBallX() >= (game[level].xTree2) && getBallX() <=(game[level].xTree2) + 2) && (getBallY()>=(game[level].yTree2) && getBallY()<= (game[level].yTree2) + 2))  //treeB
	{Sound_Hit();return TreeB;}*/
	switch (level) // checking for bounds
	{
		case 0: if((getBallX()<= 0 || getBallX()>=128) || (getBallY()<=0 || getBallY()>=160)) {Sound_Water();return Water;}
		case 1: if((getBallX()<= 0 || getBallX()>=128) || (getBallY()<=0 || getBallY()>=160)) {Sound_Water();return Water;}
		
	}
		return 0;
}


void setBounds (int LevelNumber)
{
	switch (LevelNumber)
	{
		case 1: BoundX = 0; BoundY = 0; BoundWidth = 128; BoundHeight=160; break;
		case 2: BoundX = 0; BoundY = 0; BoundWidth = 128; BoundHeight = 160; break;
			//ST7735_drawRect(BoundX, BoundY, BoundWidth, BoundHeight);
	}
}
int getSpeed(void)
{
	uint32_t data = (int)Convert(ADC_In()); // get the data from the ADC
	//Delay100ms(6);
	if(data>=5){data =5;}
	return data; 
}
int getDir(void)
	{
		int data = (int)ConvertDir(ADC_In());
	//	Delay100ms(6);
		return data; 
	}
void showSpeed(void){
		int magnitude = getSpeed();
		//int magnitude = 3;
		int xPos = getBallX() + magnitude + 17;
		int yPos = getBallY() - magnitude - 10;
		ST7735_FillRect(xPos, yPos, 2, 2, 0x0000);
		
	}
void showDir(int level)
{
	int magnitude = getSpeed(); 
	if (magnitude>=3) {magnitude =3;}
	int xPos = getBallX() ;
	int yPos = getBallY() ;
	int poserx =0;int posery =0;
	int index = getDir(); 
	//ST7735_FillRect(xPos, yPos, 2,2,0x07E0);
	//ST7735_FillRect(xPos+poserx, yPos+posery, 3,3,0x07E0);
	poserx = (run[index]) *(magnitude);
	posery = (rise[index]) *(magnitude);
		if(index<180){
			posery*=-1;}
	ST7735_FillRect(xPos+poserx, yPos+posery, 2,2,0x000);
	Delay100ms(12);
	ST7735_FillRect(xPos+poserx, yPos+posery, 2,2,0x07E0);
	//displayLevel(level);
	
	
}
void displayData(int lvl);
void displayLevel( int level){
	ST7735_FillScreen(0);
	
	ST7735_DrawBitmap(0,160,game[level-1].image,120,160);
	//setBall();
	ST7735_DrawBitmap(game[level-1].xTree1,game[level-1].yTree1,tree,12,12);
	ST7735_DrawBitmap(game[level-1].xTree2,game[level-1].yTree2,tree,12,12);
	ST7735_DrawBitmap(game[level-1].xHole,game[level-1].yHole,Hole, 7,14);

	displayData(level);
}

void displayData(int lvl){
	ST7735_SetCursor(0,0);
	ST7735_OutString("STROKES: ");
	ST7735_SetCursor(9,0);
	ST7735_OutChar(0x30+getStrokes()); //max number of strokes is 9
	
	ST7735_SetCursor(14,0);
	ST7735_OutString("HOLE: ");
	LCD_OutDec(game[lvl-1].lvlNum);
	
	ST7735_SetCursor(0,1);
	ST7735_OutString("PAR: ");
	ST7735_SetCursor(5,1);
	ST7735_OutChar(0x30+game[lvl-1].par);
	
	ST7735_SetCursor(14,14);
	ST7735_OutString("OPTIONS");
	ST7735_SetCursor(17,15);
	ST7735_OutString("EXIT");
	
}

int Convert(uint32_t input){ //Convert reading from ADC_In to distance along slide pot
  double slope = 11 * input/4095; 			//y = 0.0046806x+0.093609
	double intercept = 0;		//r = 0.992
	int result = (int) (slope + intercept);
	
	return result;}
int ConvertDir(uint32_t input)
{
	int slope = 180*input/4095;
	int intercept = 0; 
	int result  = slope +intercept;
return result; 	
}
