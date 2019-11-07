/*
 * Temp_sensor.c
 *
 *  Created on: Nov 7, 2019
 *      Author: ahmed
 */

#include "Temp_sensor.h"

volatile void(*g_callBackPtr)(void);

void TempSensor_init(void){
	ADC_init();
	TIMER_init(TIMER0);
	TIMER0_OVF_setCallBack(TempCal);
	ADC_readChannel(0);
}

void TempCal(void){
	static uint8 counter =0;
	counter++;
	if(30 == counter){
		g_ADC_value=ADC_readChannel(0);
		g_callBackPtr();
		counter=0;
	}
}

void Temp_setCallBack(void(*ptr)(void)){
	g_callBackPtr = ptr;
}

