#include "tm4c123gh6pm.h"
#include <stdint.h>
#include "ST7735.h"
#include "Sensor.h"
#include "Timer1.h"

void Delay10us(void);
void Sensor_Init(){
	//Initialize port and periodic timer interrrupt
	SYSCTL_RCGCGPIO_R |= 0x10;
	while((SYSCTL_PRGPIO_R&0x10) ==0){}
	GPIO_PORTE_DIR_R |= 0x01;
	GPIO_PORTE_AFSEL_R &= ~0x03;
	GPIO_PORTE_AMSEL_R &= ~0x03;
	GPIO_PORTE_DEN_R |= 0x3;
	
}
int getDistance(){ 
	int time = 0;
	//Send signal on TRIG (PE0)
	GPIO_PORTE_DATA_R |= 0x1;
		 int i=800;
	while(i){i--;}	
	GPIO_PORTE_DATA_R ^= 0x1;

	
	//Measure time of echo using timer
	while((GPIO_PORTE_DATA_R&= 0x2) == 0){}
	while((GPIO_PORTE_DATA_R&= 0x2) != 0){time++;}
	
	//convert to distance
	return time;
}

void screenSaver(){
	//int dist
	
	while(getDistance()>2000){
		ST7735_FillScreen(0);
		ST7735_SetCursor(0,7);
		ST7735_OutString(" SCREEN SAVER MODE ");
	}
	
}
