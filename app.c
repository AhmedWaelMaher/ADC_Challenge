/*
 * app.c
 *
 *  Created on: Nov 7, 2019
 *      Author: rgb
 */

#include "Temp_sensor.h"
#include "lcd.h"

void Fan_Speed(void){
	uint16 data = g_ADC_value*0.488;
	LCD_goToRowCol(0,10);
	LCD_intgerToString(data);
	if(data < 20){
		LCD_goToRowCol(1,10);
		LCD_intgerToString(0);
	}
	else if((data >=20)&&(data < 25)){
		LCD_goToRowCol(1,10);
		LCD_intgerToString(50);
	}
	else if((data >= 25)&&(data <50)){
		LCD_goToRowCol(1,10);
		LCD_intgerToString(80);
	}
	else{
		LCD_clearScreen();
		LCD_displayString("Power OFF");
	}
}

int main(void){
	LCD_init();
	LCD_displayString("Temp = ");
	LCD_displayStringRowCol(1,0,"Duty = ");
	TempSensor_init();
	Temp_setCallBack(Fan_Speed);
	TIMER_start(TIMER0);
	while(1){

	}
	return 0;
}
